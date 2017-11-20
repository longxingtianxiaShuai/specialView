//
//  VBPathGuider.m
//  VBPathGuider
//
//  Created by Justin on 2014. 6. 23..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import "VBPathGuider.h"
#import "MAVector.h"

@implementation VBPathGuider {
    std::vector<vbPathNode>	nodes;	//Node¿« πËø≠
    
	//»∏¿¸ º”º∫
	float		turnOffset;					//»∏¿¸¿ª Ω√¿€«“ ∞≈∏Æ ##V.2 - æ’º± Ω√º± πÊ«‚¿« ¿ßƒ°
    
	//º”µµ º”º∫
	float		speedMove;				//¿Ãµø º”µµ - «—π¯ ¿¸¡¯(æ≤∑πµÂ «—π¯ »£√‚ ¡÷±‚) «“ ∂ß ¿Ãµø «“ ∞≈∏Æ(SBMø°º≠ Export µ» ¥‹¿ß ±‚¡ÿ)
	float		speedTurn;				//»∏¿¸ º”µµ - «—π¯ »∏¿¸(æ≤∑πµÂ «—π¯ »£√‚ ¡÷±‚) «“ ∂ß »∏¿¸ «“ ∞¢µµ(Degree)
    
	//ªÛ≈¬ º”º∫
	vbPath_NaviMode	naviMode;		//Navigation mode //##V.2
	vbPath_State		curState;			//«ˆ¿Á ªÛ≈¬
	vbPath_State		nextState;			//¿œΩ√ ¡§¡ˆ ¿Ã»ƒ ¡¯«‡ «“ ªÛ≈¬(∏  ∑Œµ˘ µÓ Pause∏¶ ªÁøÎ «“ ∂ß ¥ŸΩ√ Ω√¿€«“ ∏µÂ∏¶ ∞·¡§«—¥Ÿ.)
	int					curNodeIdx;		//«ˆ¿Á ¡¯«‡ ¡ﬂ¿Œ Node¿« Index(IDæ∆¥‘) (Move¿œ ∂ß¥¬ √‚πﬂ¡° Node¿« index, Turn¿œ ∂ß¥¬ ¿ßƒ°«œ∞Ì ¿÷¥¬ Node¿« index)
	vec3				curPos;				//«ˆ¿Á ¿ßƒ°
	float				curDirDeg;			//«ˆ¿Á πÊ«‚(±‚¡ÿ πÊ«‚¿∏∑Œ∫Œ≈Õ »∏¿¸ ∞¢µµ(Degree)∏¶ ¿«πÃ«‘. øπ∏¶ µÈ∏È -Z√‡ πÊ«‚¿∏∑Œ∫Œ≈Õ Y√‡¿ª ±‚¡ÿ¿∏∑Œ Ω√∞ËπÊ«‚¿« ∞¢µµ)
	vec3				curDirVec;			//«ˆ¿Á πÊ«‚(¥‹¿ß ∫§≈Õ)
	int					curFloor;			//«ˆ¿Á √˛
    
	vec3				aheadPos;
}

- (id)init
{
    self = [super init];
    
    if(self){
		naviMode = vbPath_NaviMode_BezierTurn;
    }
    return self;
}

-(id) initWithNaviMode:(vbPath_NaviMode)mode
{
    self = [super init];
    if (self) {
        naviMode = mode;
    }
    return self;
}

- (vec3) getCurPos
{
    return curPos;
}

- (float) getCurDirDeg
{
    return curDirDeg;
}

- (void) setSpeed:(float)move turn:(float)turn
{
    speedMove = move;
    speedTurn = turn;
}

- (void) setSpeedAndTurnOffset:(float)move offset:(float)offset
{
    speedMove = move;
    turnOffset = offset;
}

- (BOOL) setPathNode:(std::vector<vbPathNode> *)newPath
{
    if(newPath->size()<2)
    {
        printf("Path should have more than 2 nodes.\n");
        return NO;
    }
    
    //±‚¡∏ ∞Ê∑Œ ¡§∫∏ ªË¡¶
    nodes.resize(0);
    
    //∞Ê∑Œ Node ∫πªÁ
    for(unsigned int i=0; i<newPath->size(); i++)
    {
        nodes.push_back(newPath->at(i));
        
        if(i>0)
        {
            vbPathNode	prvNode = nodes.at(i-1);
            vec3 dir = (newPath->at(i).pos - prvNode.pos);
            prvNode.dirDeg = [self getDirDegree:dir];
            prvNode.dirVec = dir.Normalized();
            nodes[i-1] = prvNode;
        }
    }
    
    return YES;
}

- (void) initGuide
{
    if(nodes.size()<2)
    {
        printf("Path should have more than 2 nodes.\n");
        return;
    }
    
    //√‚πﬂ ¡ÿ∫Ò
    curState	=	vbPath_State_Set;
    
    //«ˆ¿Á Node index = 0
    curNodeIdx = 0;
    vbPathNode		startNode = nodes.at(0);
    vbPathNode		nextNode = nodes.at(1);
    
    //«ˆ¿Á ¿ßƒ° = √ππ¯¬∞ Node¿« ¿ßƒ°
    curPos = startNode.pos;
    
    //«ˆ¿Á √˛
    curFloor	= startNode.floor;
    
    //«ˆ¿Á πÊ«‚ = (NextNoe - startNode) πÊ«‚¿ª Dir∑Œ º≥¡§
    curDirVec	= (nextNode.pos - startNode.pos).Normalized();
    curDirDeg	= [self getDirDegree:curDirVec];
    
    //Ω√º± πÊ«‚ ¿ßƒ° #V.2 ######################
    [self updateAheadPosAndOrientation];
    
}

- (void) doNextStep
{
    switch(naviMode)
    {
		case vbPath_NaviMode_TurnAndGo:
			[self doNextStepTurnAndGo];
			break;
            
		case vbPath_NaviMode_BezierTurn:
			[self doNextStepTurnBezier];
			break;
    }
}

- (void) doNextStepTurnBezier
{
    switch(curState)
    {
		case vbPath_State_Set://√‚πﬂ¥Î±‚ ªÛ≈¬
        {
            curState	=	vbPath_State_Move;				//¿Ãµø ªÛ≈¬∑Œ πŸ≤€¥Ÿ.
            printf("[STATE CHANGE] Set -> Move\n");
            printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
        }
			break;
            
		case vbPath_State_Move://¥Ÿ∏• Node∑Œ ¿Ãµø ¡ﬂ¿Œ ªÛ≈¬
        {
            vbPathNode	curNode = nodes.at(curNodeIdx);
            vbPathNode	nextNode;
            if (curNodeIdx+1 < nodes.size()) {
                nextNode = nodes.at(curNodeIdx+1);
            } else {
                nextNode = nodes.at(curNodeIdx);
            }
            
            
            curPos		=	curPos + curNode.dirVec	* speedMove;	//ªı∑ŒøÓ ¿ßƒ°¥¬ ±‚¡∏ ¿ßƒ°ø° ¿Ãµø πÊ«‚ø° º”µµ∏¶ ∞ˆ«— ∞™¿ª ¥ı«— ¿ßƒ°∞° µ»¥Ÿ.
            
            //πÊ«‚ ∞ªΩ≈
            [self updateAheadPosAndOrientation];
            
            //¿Ãµø »ƒ Nodeø° µµ¥ﬁ «ﬂ¿ª ∂ß¿« √≥∏Æ
            if([self isArrivedNextNode:curPos nextPos:nextNode.pos] || curNodeIdx+1 == nodes.size())
            {
                //∏Ò¿˚¡ˆ µµ¬¯/»∏¿¸/√˛∫Ø∞Ê ¡ﬂ æÓ∂≤ ∞ÊøÏø° «ÿ¥Á«œ¥¬¡ˆ ∆«¥‹
                
                //∏Ò¿˚¡ˆ µµ¬¯ = CurNodeIdx+2 >= Node ∞≥ºˆ
                if((unsigned int)(curNodeIdx+2)>=nodes.size())
                {
                    curState	= vbPath_State_Arrive;		//µµ¬¯ø° ¥Î«— √≥∏Æ¥¬ ¥Ÿ¿Ω ∑Á«¡ø°º≠ µµ¬¯ ªÛ≈¬∑Œ ¡¯¿‘«œø© √≥∏Æ
                    printf("[STATE CHANGE] Move -> Arrive\n");
                    printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                }
                //∏Ò¿˚¡ˆ µµ¬¯¿Ã æ∆¥œ∂Û∏È √˛ ∫Ø∞Ê »§¿∫ ªı∑ŒøÓ ≥ÎµÂø°º≠ ¿Ãµø
                else
                {
                    //µµ¬¯¿Ã æ∆¥œ∂Û∏È nextNode ¥Ÿ¿Ωø° ≥ÎµÂ∞° ¥ı ¿÷¥Ÿ.
                    vbPathNode	nextnextNode =  nodes.at(curNodeIdx+2);
                    
                    //√˛¿Ã ¥Ÿ∏• ≥ÎµÂ = √˛ ∫Ø∞Ê √≥∏Æ
                    if(nextnextNode.floor!=nextNode.floor)
                    {
                        curState	= vbPath_State_UpDown;		//√˛ ∫Ø∞Ê ªÛ≈¬∑Œ ∫Ø∞Ê - √˛ ∫Ø∞Ê √≥∏Æ¥¬ √˛ ∫Ø∞Ê ªÛ≈¬∑Œ ¡¯¿‘«œø© √≥∏Æ«‘.
                        printf("[STATE CHANGE] Move -> UpDown\n");
                        printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                    }
                    //∏Ò¿˚¡ˆ µµ¬¯¿Ã æ∆¥œ∞Ì √˛ ∫Ø∞Ê¿Ã æ∆¥œ∏È ªı∑ŒøÓ ≥ÎµÂø°º≠ ¿Ãµø
                    else
                    {
                        printf("[STATE CHANGE] Move to Next Node\n");
                        printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                    }
                }
            
                curNodeIdx++;					//µµ¬¯«— Node¿« Index
                curPos = nextNode.pos;		//µµ¬¯«— Node¿« ¿ßƒ°
            }
            if ([self.delegate respondsToSelector:@selector(onChangedCurPos:state:)]) {
                MAVector *v = [[MAVector alloc] init];
                v.x = curPos.x;
                v.y = curPos.z * -1.0;
                v.z = curPos.y;
                
                [self.delegate onChangedCurPos:v state:curState];
            }

        }
			break;
            
		case vbPath_State_UpDown://¥Ÿ∏• √˛¿∏∑Œ ∫Ø∞Ê ¿¸ ªÛ≈¬
        {
            //«ˆ¿Á ≥ÎµÂ¥¬ √˛ ∫Ø∞Ê¿ª «œ±‚ ¿¸¿« Nodeø° ¿÷¿Ω.
            //vbPathNode	curNode		= nodes.at(curNodeIdx);			//«ˆ¿Á Node
            vbPathNode	nextNode	= nodes.at(curNodeIdx+1);		//¥Ÿ¿Ω Node
            
            //¥Ÿ¿Ω ≥ÎµÂ¿« ¿ßƒ°∑Œ ¿Ãµø
            curPos	= nextNode.pos;
            [self updateAheadPosAndOrientation];
            
            //¥Ÿ¿Ω ≥ÎµÂø°º≠¿« πÊ«‚ ∞·¡§
            if((unsigned int)(curNodeIdx+1)>=nodes.size())////√˛ ∫Ø∞Ê »ƒ¿« ≥ÎµÂ∞° ∏∂¡ˆ∏∑ ≥ÎµÂ∂Û∏È..
            {
                curState = vbPath_State_Pause;
                nextState = vbPath_State_Arrive;	//∏  ∑Œµ˘ »ƒ µµ¬¯ √≥∏Æ «‘.
                
                printf("[STATE CHANGE] Updown -> Pause\n");
                printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
            }
            else
            {
                curState = vbPath_State_Pause;
                nextState = vbPath_State_Move;	//∏  ∑Œµ˘ »ƒ ¡¯«‡¿ª «“ ºˆ ¿÷µµ∑œ «‘.
                printf("[STATE CHANGE] Updown -> Pause\n");
                printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
            }
            if ([self.delegate respondsToSelector:@selector(onChangedCurPos:state:)]) {
                MAVector *v = [[MAVector alloc] init];
                v.x = curPos.x;
                v.y = curPos.z * -1.0;
                v.z = curPos.y;
                
                [self.delegate onChangedCurPos:v state:curState];
            }

            //¥Ÿ¿Ω √˛¿« ≥ÎµÂ∏¶ ¿–µµ∑œ «œ∞Ì, ¥Ÿ¿Ω √˛ø°º≠ ¿Ãµø¿ª «“ ºˆ ¿÷¥¬ ªÛ≈¬∏¶ ∏∏µÈæÓ¡‡æﬂ «‘.
            //∏  ∑Œµ˘¿ª ∫∞µµ¿« æ≤∑πµÂ∑Œ ±∏«ˆ «—¥Ÿ∏È, ¿Ã ≈¨∑°Ω∫¿« ªÛ≈¬¥¬ Pause∑Œ «œ∞Ì, ¥Ÿ¿Ω ªÛ≈¬∏¶ µµ¬¯¿Ã≥™ ¿Ãµø¿∏∑Œ º≥¡§ «ÿ µŒ∞Ì, ∑Œµ˘ øœ∑· »ƒ ¥Ÿ¿Ω ªÛ≈¬∏¶ «ˆ¿Á ªÛ≈¬∑Œ π›øµ«œø© æ≤∑πµÂ ¡¯«‡«—¥Ÿ.
            
            //∏  ∑Œµ˘ ƒ⁄µÂ...µ®∏Æ∞‘¿Ã∆Æ∏¶ ¥ﬁ¥¯¡ˆ πª «œ¥¯¡ˆ... ∞≥πﬂ »Ø∞Êø° ∏¬∞‘.....
            //loadMap(∫Ø∞Ê µ» √˛¿« ∏ )
            
            //∫Ø∞Ê µ» √˛¿« Ω√¿€ Node∑Œ «ˆ¿Á Node º≥¡§
            curNodeIdx++;
        }
			break;
            
		case vbPath_State_Arrive:
        {
            if ([self.delegate respondsToSelector:@selector(onGuidanceFinished)]) {
                [self.delegate onGuidanceFinished];
            }
        }
			break;
            
		case vbPath_State_Pause:
            if ([self.delegate respondsToSelector:@selector(onGuidancePause)]) {
                [self.delegate onGuidancePause];
            }
            
            break;
		default:
        {
            //æ∆π´∞Õµµ «œ¡ˆ æ ¿Ω. ###################################### ∏  ∑Œµ˘ µÓ..... ResolvePauseState()∏¶ »£√‚«œ∏È Pause∞° «ÿ¡¶µ«æÓ πÃ∏Æ º≥¡§ µ» ∏µÂ∑Œ ¡¯¿‘
        }
			break;
    }
}

- (void) doNextStepTurnAndGo
{
    switch(curState)
    {
		case vbPath_State_Set://√‚πﬂ¥Î±‚ ªÛ≈¬
        {
            curState	=	vbPath_State_Move;				//¿Ãµø ªÛ≈¬∑Œ πŸ≤€¥Ÿ.
            printf("[STATE CHANGE] Set -> Move\n");
            printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
        }
			break;
            
		case vbPath_State_Move://¥Ÿ∏• Node∑Œ ¿Ãµø ¡ﬂ¿Œ ªÛ≈¬
        {
            curPos		=	curPos + curDirVec	* speedMove;	//ªı∑ŒøÓ ¿ßƒ°¥¬ ±‚¡∏ ¿ßƒ°ø° ¿Ãµø πÊ«‚ø° º”µµ∏¶ ∞ˆ«— ∞™¿ª ¥ı«— ¿ßƒ°∞° µ»¥Ÿ.
            vbPathNode	nextNode = nodes.at(curNodeIdx+1);
            
            //¿Ãµø »ƒ Nodeø° µµ¥ﬁ «ﬂ¿ª ∂ß¿« √≥∏Æ
            if([self isArrivedNextNode:curPos nextPos:nextNode.pos])
            {
                //∏Ò¿˚¡ˆ µµ¬¯/»∏¿¸/√˛∫Ø∞Ê ¡ﬂ æÓ∂≤ ∞ÊøÏø° «ÿ¥Á«œ¥¬¡ˆ ∆«¥‹
                
                
                //∏Ò¿˚¡ˆ µµ¬¯ = CurNodeIdx+2 >= Node ∞≥ºˆ
                if((unsigned int)(curNodeIdx+2)>=nodes.size())
                {
                    curState	= vbPath_State_Arrive;		//µµ¬¯ø° ¥Î«— √≥∏Æ¥¬ ¥Ÿ¿Ω ∑Á«¡ø°º≠ µµ¬¯ ªÛ≈¬∑Œ ¡¯¿‘«œø© √≥∏Æ
                    printf("[STATE CHANGE] Move -> Arrive\n");
                    printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                }
                //∏Ò¿˚¡ˆ µµ¬¯¿Ã æ∆¥œ∂Û∏È √˛ ∫Ø∞Ê »§¿∫ πÊ«‚ ¿¸»Ø¿”.
                else
                {
                    //µµ¬¯¿Ã æ∆¥œ∂Û∏È nextNode ¥Ÿ¿Ωø° ≥ÎµÂ∞° ¥ı ¿÷¥Ÿ.
                    vbPathNode	nextnextNode =  nodes.at(curNodeIdx+2);
                    
                    //√˛¿Ã ¥Ÿ∏• ≥ÎµÂ = √˛ ∫Ø∞Ê √≥∏Æ
                    if(nextnextNode.floor!=nextNode.floor)
                    {
                        curState	= vbPath_State_UpDown;		//√˛ ∫Ø∞Ê ªÛ≈¬∑Œ ∫Ø∞Ê - √˛ ∫Ø∞Ê √≥∏Æ¥¬ √˛ ∫Ø∞Ê ªÛ≈¬∑Œ ¡¯¿‘«œø© √≥∏Æ«‘.
                        printf("[STATE CHANGE] Move -> UpDown\n");
                        printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                    }
                    //∏Ò¿˚¡ˆ µµ¬¯¿Ã æ∆¥œ∞Ì √˛ ∫Ø∞Ê¿Ã æ∆¥œ∏È πÊ«‚ ¿¸»Ø¿∏∑Œ ∆«¥‹.
                    else
                    {
                        curState	=	vbPath_State_Turn;			//πÊ«‚ ¿¸»Ø ªÛ≈¬∑Œ ∫Ø∞Ê - πÊ«‚ ¿¸»Ø √≥∏Æ¥¬ πÊ«‚ ¿¸»Ø ªÛ≈¬∑Œ ¡¯¿‘«œø© √≥∏Æ«‘.
                        printf("[STATE CHANGE] Move -> Turn\n");
                        printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                        
                        [self updateTurnSpeedSign:curDirDeg nextDirDeg:nextNode.dirDeg];		//»∏¿¸ πÊ«‚¿ª ∞·¡§
                    }
                }
                
                curNodeIdx ++;					//µµ¬¯«— Node¿« Index
                curPos = nextNode.pos;		//µµ¬¯«— Node¿« ¿ßƒ°
            }
            if ([self.delegate respondsToSelector:@selector(onChangedCurPos:state:)]) {
                MAVector *v = [[MAVector alloc] init];
                v.x = curPos.x;
                v.y = curPos.z * -1.0;
                v.z = curPos.y;
                
                [self.delegate onChangedCurPos:v state:curState];
            }

        }
			break;
            
		case vbPath_State_Turn://Nodeø°º≠ »∏¿¸ ¡ﬂ¿Œ ªÛ≈¬
        {
            //∏Ò«• ∞¢µµ(«ˆ¿Á ≥ÎµÂ¿« πÊ«‚)ø° µµ¥ﬁ«ﬂ¿∏∏È ¥Ÿ¿Ω ªÛ≈¬∑Œ ∫Ø∞Ê - »∏¿¸ »ƒ ø√ ºˆ ¿÷¥¬ ªÛ≈¬¥¬ ¿Ãµø ª”¿”.
            vbPathNode	curNode		= nodes.at(curNodeIdx);			//«ˆ¿Á Node
            if([self isHeadedToNextNode:curDirDeg nextDirDeg:curNode.dirDeg])		//«ˆ¿Á πÊ«‚∞˙ «ˆ¿Á ≥ÎµÂø°º≠ ¥Ÿ¿Ω ≥ÎµÂ∏¶ «‚«œ¥¬ πÊ«‚¿Ã ∞°±ÓøÓ¡ˆ ∆«¥‹
            {
                //∞°±ı¥Ÿ∏È »∏¿¸ ªÛ≈¬∏¶ ∏ÿ√ﬂ∞Ì ¥Ÿ¿Ω ≥ÎµÂø°º≠ ¡¯«‡ «“ ºˆ ¿÷µµ∑œ «‘.
                curState	= vbPath_State_Move;		//»∏¿¸ ¿Ã»ƒø°¥¬ ¡¯«‡∏∏ «‘.
                curDirDeg = curNode.dirDeg;		//¡§»Æ»˜ ¥Ÿ¿Ω ≥ÎµÂ∏¶ «‚«œµµ∑œ πÊ«‚¿ª º≥¡§«‘.
                curDirVec = curNode.dirVec;		//¥Ÿ¿Ω ≥ÎµÂ∏¶ «‚«œø© ¿Ãµø«œµµ∑œ «‘.
                
                printf("[STATE CHANGE] Turn -> Move\n");
                printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
                break;
            }
            
            //»∏¿¸ πÊ«‚¿∫ Turn mode∑Œ ∫Ø∞Ê µ«∏Èº≠ ∞·¡§ µ«æÓ ¿÷¿∏π«∑Œ, ±◊≥… ¥ı«ÿ¡÷∏È µ»¥Ÿ.
            curDirDeg += speedTurn;		//»∏¿¸ º”µµ∏¶ ¥ı«œø© «ˆ¿Á º”µµ∏¶ ∞ªΩ≈«—¥Ÿ.
            
            //∞¢µµ π¸¿ß∏¶ π˛æÓ≥™¥¬ ∞ÊøÏø° ¥Î«— √≥∏Æ - ∞¢µµ π¸¿ß∏¶ 0~360¿∏∑Œ ¿Ø¡ˆ
            if(curDirDeg<0.f)				curDirDeg += 360.f;		//¿Ωºˆ∑Œ ¡¯¿‘«œ∏È 360¿ª ¥ı«ÿ¡ÿ¥Ÿ.
            else if(curDirDeg>360.f)       curDirDeg -= 360.f;		//360¿ª ≥—æÓ∞°∏È 360¿ª ª©¡ÿ¥Ÿ.
            
            if ([self.delegate respondsToSelector:@selector(onChangedCurPos:state:)]) {
                MAVector *v = [[MAVector alloc] init];
                v.x = curPos.x;
                v.y = curPos.z * -1.0;
                v.z = curPos.y;
                
                [self.delegate onChangedCurPos:v state:curState];
            }

        }
			break;
            
		case vbPath_State_UpDown://¥Ÿ∏• √˛¿∏∑Œ ∫Ø∞Ê ¿¸ ªÛ≈¬
        {
            //«ˆ¿Á ≥ÎµÂ¥¬ √˛ ∫Ø∞Ê¿ª «œ±‚ ¿¸¿« Nodeø° ¿÷¿Ω.
            vbPathNode	curNode		= nodes.at(curNodeIdx);			//«ˆ¿Á Node
            vbPathNode	nextNode	= nodes.at(curNodeIdx+1);		//¥Ÿ¿Ω Node
            
            //¥Ÿ¿Ω ≥ÎµÂ¿« ¿ßƒ°∑Œ ¿Ãµø
            curPos	= nextNode.pos;
            
            //¥Ÿ¿Ω ≥ÎµÂø°º≠¿« πÊ«‚ ∞·¡§
            if((unsigned int)(curNodeIdx+1)>=nodes.size())////√˛ ∫Ø∞Ê »ƒ¿« ≥ÎµÂ∞° ∏∂¡ˆ∏∑ ≥ÎµÂ∂Û∏È..
            {
                //√˛ ∫Ø∞Ê »ƒ Node∞° «œ≥™ ª”¿Ã∂Û∏È πÊ«‚¿ª ∞·¡§ «“ ºˆ æ¯¿∏π«∑Œ, ¿ßƒ°¥¬ ¥Ÿ¿Ω Node∏¶ ¬¸¡∂«œ∞Ì, πÊ«‚¿∫ «ˆ¿Á¿« πÊ«‚¿ª ¿Ø¡ˆ«œµµ∑œ «—¥Ÿ.
                curDirDeg = curNode.dirDeg;
                
                curState = vbPath_State_Pause;
                nextState = vbPath_State_Arrive;	//∏  ∑Œµ˘ »ƒ µµ¬¯ √≥∏Æ «‘.
                
                printf("[STATE CHANGE] Updown -> Pause\n");
                printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
            }
            else
            {
                //≥ÎµÂ∞° ¥ı ¿÷¥Ÿ∏È, ¥Ÿ¿Ω ≥ÎµÂ¿« πÊ«‚¿ª ªÁøÎ«œ∏È µ .
                curDirDeg = nextNode.dirDeg;
                
                curState = vbPath_State_Pause;
                nextState = vbPath_State_Move;	//∏  ∑Œµ˘ »ƒ ¡¯«‡¿ª «“ ºˆ ¿÷µµ∑œ «‘.
                printf("[STATE CHANGE] Updown -> Pause\n");
                printf("[Pos] %f,%f,%f\t\t[DirDeg] %f]\n",curPos.x,curPos.y,curPos.z,curDirDeg);
            }
            
            //¥Ÿ¿Ω √˛¿« ≥ÎµÂ∏¶ ¿–µµ∑œ «œ∞Ì, ¥Ÿ¿Ω √˛ø°º≠ ¿Ãµø¿ª «“ ºˆ ¿÷¥¬ ªÛ≈¬∏¶ ∏∏µÈæÓ¡‡æﬂ «‘.
            //∏  ∑Œµ˘¿ª ∫∞µµ¿« æ≤∑πµÂ∑Œ ±∏«ˆ «—¥Ÿ∏È, ¿Ã ≈¨∑°Ω∫¿« ªÛ≈¬¥¬ Pause∑Œ «œ∞Ì, ¥Ÿ¿Ω ªÛ≈¬∏¶ µµ¬¯¿Ã≥™ ¿Ãµø¿∏∑Œ º≥¡§ «ÿ µŒ∞Ì, ∑Œµ˘ øœ∑· »ƒ ¥Ÿ¿Ω ªÛ≈¬∏¶ «ˆ¿Á ªÛ≈¬∑Œ π›øµ«œø© æ≤∑πµÂ ¡¯«‡«—¥Ÿ.
            
            //∏  ∑Œµ˘ ƒ⁄µÂ...µ®∏Æ∞‘¿Ã∆Æ∏¶ ¥ﬁ¥¯¡ˆ πª «œ¥¯¡ˆ... ∞≥πﬂ »Ø∞Êø° ∏¬∞‘.....
            //loadMap(∫Ø∞Ê µ» √˛¿« ∏ )
            
            //∫Ø∞Ê µ» √˛¿« Ω√¿€ Node∑Œ «ˆ¿Á Node º≥¡§
            if ([self.delegate respondsToSelector:@selector(onChangedCurPos:state:)]) {
                MAVector *v = [[MAVector alloc] init];
                v.x = curPos.x;
                v.y = curPos.z * -1.0;
                v.z = curPos.y;
                
                [self.delegate onChangedCurPos:v state:curState];
            }

            curNodeIdx++;
        }
			break;
            
		case vbPath_State_Arrive:
        {
            //µµ¬¯ «ﬂ¿ª ∂ß¿« √≥∏Æ #####################################
        }
			break;
            
		case vbPath_State_Pause:
		default:
        {
            //æ∆π´∞Õµµ «œ¡ˆ æ ¿Ω. ###################################### ∏  ∑Œµ˘ µÓ..... ResolvePauseState()∏¶ »£√‚«œ∏È Pause∞° «ÿ¡¶µ«æÓ πÃ∏Æ º≥¡§ µ» ∏µÂ∑Œ ¡¯¿‘
        }
			break;
    }
}

- (void) resolvePauseState
{
    curState = nextState;
    switch(curState)
    {
		case vbPath_State_Set	:		printf("[STATE CHANGE] Pause -> Set\n");		break;
		case vbPath_State_Move:		printf("[STATE CHANGE] Pause -> Move\n");		break;
		case vbPath_State_Turn:		printf("[STATE CHANGE] Pause -> Turn\n");		break;
		case vbPath_State_UpDown:	printf("[STATE CHANGE] Pause -> UpDown\n");		break;
		case vbPath_State_Arrive:		printf("[STATE CHANGE] Pause -> Arrive\n");		break;
		case vbPath_State_Pause:		printf("[STATE CHANGE] Pause -> Pause\n");		break;
    }
}

- (float) getDirDegreeFrom:(vbPathNode)fromNode toNode:(vbPathNode)toNode
{
    vec3	dirVector = toNode.pos - fromNode.pos;	//πÊ«‚ = ¥Ÿ¿Ω ≥ÎµÂ ¿ßƒ° - √‚πﬂ ≥ÎµÂ ¿ßƒ°
    return	[self getDirDegree:dirVector];
}

- (float) getDirDegree:(vec3) dirVector
{
    float	dir_deg = (float)atan2f(-dirVector.z,dirVector.x) * 180.f / 3.141592f;
    
    //-180~180¿ª 0~360¿∏∑Œ ∫Ø»Ø
    if(dir_deg<0.f)			dir_deg += 360.f;// - dir_deg;
    else if(dir_deg>360.f)	dir_deg -= 360.f;
    
    //-Z√‡¿ª ±‚¡ÿ¿∏∑Œ Ω√∞ËπÊ«‚¿∏∑Œ «•«ˆµ«¥¬ ∞¢µµ∑Œ ∫Ø∞Ê
    float map_dir = -dir_deg + 90.f;
    if(map_dir<0)	map_dir += 360.f; //0~360 ±∏∞£¿∏∑Œ ∫Ø∞Ê
    
    return	map_dir;
}

- (BOOL) isArrivedNextNode:(vec3)pos nextPos:(vec3)nextNodePos
{
    float	dist = (pos - nextNodePos).size();
    
    if(dist<speedMove)	return YES;		//∞°±ÓøÓ ∞≈∏Æø° ¿÷¿∏∏È true
    return NO;
}

- (BOOL) isHeadedToNextNode:(float)dirDeg nextDirDeg:(float)nextDirDeg
{
    float	deviation = fabs(dirDeg - nextDirDeg);
    
    if(deviation<fabs(speedTurn))	return YES;		//∞°±ÓøÓ πÊ«‚ø° ¿÷¿∏∏È true
    return NO;
}

- (void) updateTurnSpeedSign:(float)dirDeg nextDirDeg:(float)nextDirDeg
{
    if(dirDeg>=0.f && dirDeg<180.f)
    {
        if(dirDeg<nextDirDeg && (dirDeg+180.f)>nextDirDeg)
            speedTurn = fabs(speedTurn);
        else
            speedTurn = -fabs(speedTurn);
    }
    //«ˆ¿Á πÊ«‚ = [180 ~ 360]
    else
    {
        if(dirDeg<nextDirDeg || (dirDeg-180.f)>nextDirDeg)
            speedTurn = fabs(speedTurn);
        else
            speedTurn = -fabs(speedTurn);
    }
}

- (void) updateAheadPosAndOrientation
{
    //vbPathNode	curNode		= nodes.at(curNodeIdx);			//«ˆ¿Á Node
    //vbPathNode	nextNode	= nodes.at(curNodeIdx+1);		//¥Ÿ¿Ω Node
    
    vbPathNode	nextNode;
    if (curNodeIdx+1 < nodes.size()) {
        nextNode = nodes.at(curNodeIdx+1);
    } else {
        nextNode = nodes.at(curNodeIdx);
    }

    
    //∏∂¡ˆ∏∑ ∞Ê∑Œø° ¿÷¥Ÿ∏È ∏Ò«• ¿ßƒ°¥¬ ∏∂¡ˆ∏∑ Node ¿ßƒ°∑Œ «—¥Ÿ.
    if((unsigned int)curNodeIdx>=nodes.size()-2)
    {
        aheadPos = nodes.at(nodes.size()-1).pos;	//∏∂¡ˆ∏∑ Node¿« ¿ßƒ°
    }
    else//≥≤¿∫ ∞≈∏Æ∏¶ ∞®æ»«œø© ∏Ò«• ¿ßƒ° ∞·¡§
    {
        //«ˆ¿Á ¿ßƒ°ø°º≠ ¥Ÿ¿Ω Node±Ó¡ˆ ∞≈∏Æ
        float fRestDistanceToNextNode = (nextNode.pos - curPos).size();
        
        //«ˆ¿Á ≥ÎµÂø° ≥≤¿∫ ∞≈∏Æ∞° Offset∫∏¥Ÿ ≈©∏È(»∏¿¸ ¿¸) ¥Ÿ¿Ω ≥ÎµÂ∏¶ «‚«œµµ∑œ «‘.
        if(fRestDistanceToNextNode>=turnOffset)
        {
            aheadPos = nextNode.pos;
        }
        else//¥Ÿ¿Ω ≥ÎµÂø° ∏Ò«• ¿ßƒ°∞° ¿÷¥¬ ∞ÊøÏ
        {
            //«ˆ¿Á ø°¡ˆø°º≠ Offset ∞≈∏Æ∏¶ ª´ ≥≤¿∫ Offset ∞≈∏Æ
            float restOffset = turnOffset - fRestDistanceToNextNode;
            
            //≥≤¿∫ ∞≈∏Æ∞° 0¿Ã µ…∂ß±Ó¡ˆ æ’º± ø°¡ˆ∏¶ ≈Ωªˆ «ÿ ≥™∞£¥Ÿ.
            int search_node_offset = 1;
            while(restOffset>0.f && curNodeIdx+search_node_offset+1<nodes.size())
            {
                
                vbPathNode	srcNode		= nodes.at(curNodeIdx+search_node_offset);			//«ˆ¿Á Node
                vbPathNode	srcNextNode	= nodes.at(curNodeIdx+search_node_offset+1);		//¥Ÿ¿Ω Node
                float	nexEdgeLength		= (srcNode.pos - srcNextNode.pos).size();
                
                //¥Ÿ¿Ω ø°¡ˆ ¿¸√º∏¶ ¥Ÿ ª©µµ µ»¥Ÿ∏È ±◊≥… ¿¸√º ±Ê¿Ã∏¶ ¥Ÿ ª©∞Ì ±◊ ¥Ÿ¿Ω ≥ÎµÂ∏¶ ∞ÀªÁ«—¥Ÿ.
                if(nexEdgeLength<restOffset)
                {
                    //∏∏æ‡, ¥Ÿ¿Ω Edge∞° ∏∂¡ˆ∏∑ Edge∂Û∏È Offset ±Ê¿Ã∏¶ ª©¡ˆ æ ∞Ì, ±◊≥… ∏∂¡ˆ∏∑ Node∏¶ ∏Ò«• ¿ßƒ°∑Œ ªÔ¥¬¥Ÿ.
                    if(curNodeIdx==nodes.size()-3)
                    {
                        restOffset = -1.f;	//∑Á«¡ ≈ª√‚ ¡∂∞«
                        aheadPos = nodes.at(nodes.size()-1).pos;	//∏∂¡ˆ∏∑ Node¿« ¿ßƒ°
                        break;
                    }
                    else//¥Ÿ¿Ω ø°¡ˆ∞° ∏∂¡ˆ∏∑ ø°¡ˆ∞° æ∆¥œ∂Û∏È, ≥≤¿∫ ∞≈∏Æ∏¶ ¬˜∞®«œ∞Ì ¥Ÿ¿Ω ø°¡ˆ∏¶ ∞ÀªÁ«œµµ∑œ «—¥Ÿ.
                    {
                        restOffset -= nexEdgeLength;
                        search_node_offset++;
                    }
                }
                else//¥Ÿ¿Ω ø°¡ˆø°º≠ ≥≤¿∫ Offset ∞≈∏Æ∏¶ ¬˜∞® «“ ºˆ ¿÷¥Ÿ∏È ≥≤¿∫ ∞≈∏Æ∏∏≈≠ ¥Ÿ¿Ω ≥ÎµÂø°º≠ ¿¸¡¯«— ¿ßƒ°∏¶ ±‚¡ÿ¿∏∑Œ «—¥Ÿ.
                {
                    //¿Ã ∏∏≈≠ ¥Ÿ¿Ω Edgeø°º≠ ¿¸¡¯«— ¿ßƒ°∏¶ Target¿∏∑Œ ªÔ¥¬¥Ÿ.
                    aheadPos = srcNode.pos + srcNode.dirVec * restOffset;
                    restOffset = -1.f;
                    break;
                }
            }//while
            
        }//¥Ÿ¿Ω ≥ÎµÂø° ∏Ò«• ¿ßƒ°∞° ¿÷¥¬ ∞ÊøÏ
    }//∏∂¡ˆ∏∑ ø°¡ˆø° ¿÷¡ˆ æ ¿∫ ∞ÊøÏ
    
    //∏Ò«• ¿ßƒ° ∞·¡§ - mAheadPosøÕ «ˆ¿Á ¿ßƒ°∏¶ ¿ÃøÎ«œø© πÊ«‚¿ª ∞·¡§«‘.
    vec3 dir = (aheadPos - curPos);
    curDirDeg = [self getDirDegree:dir];
    curDirVec = dir.Normalized();
    
    printf("[Deg] %f\n",curDirDeg);
}

@end
