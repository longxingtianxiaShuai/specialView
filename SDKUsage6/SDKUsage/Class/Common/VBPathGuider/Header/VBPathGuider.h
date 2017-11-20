//
//  VBPathGuider.h
//  VBPathGuider
//
//  Created by Justin on 2014. 6. 23..
//  Copyright (c) 2014년 Justin. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Vector.hpp" 
//¡¬«•∏¶ ≥™≈∏≥ª¥¬ vec3∏¶ ªÁøÎ«œ±‚ ¿ß«ÿº≠. ∞¢ ƒ⁄µÂø°º≠ ªÁøÎ«œ¥¬ ¡¬«• ≈∏¿‘¿ª æ≤∏È µ .
#include <vector> //vector∏¶ ªÁøÎ«œ±‚ ¿ß«ÿ

@class MAVector;

enum	vbPath_Node_Type				//Node¿« ¡æ∑˘
{
    vbPath_Node_Space	= 0,		//¿œπ›∞¯∞£
    vbPath_Node_Elevator	= 1,		//ø§∏Æ∫£¿Ã≈Õ
    vbPath_Node_Gate		= 2,		//πÆ
    vbPath_Node_Stair		= 3		//∞Ë¥‹
};

//##V.2
enum vbPath_NaviMode				//∏¿« ¡÷«‡ ∏µÂ
{
    vbPath_NaviMode_TurnAndGo	=	0,	//¿Ãµø∞˙ »∏¿¸ ∫–∏Æ
    vbPath_NaviMode_BezierTurn	=	1,	//∞Ëº” ¿Ãµø«œ∏Á, Edge∏¶ Bezier curve πÊ«‚¿∏∑Œ Ω√º± √≥∏Æ
};
//##V.2

typedef enum							//«ˆ¿Á ªÛ≈¬
{
    vbPath_State_Set		= 0,		//√‚πﬂ ¡ˆ¡°ø° ∏ÿ√Á ¿÷¥¬ ªÛ≈¬
    vbPath_State_Move	= 1,		//¥Ÿ∏• Node∑Œ ¿Ãµø ¡ﬂ¿Œ ªÛ≈¬
    vbPath_State_Turn		= 2,		//Nodeø°º≠ »∏¿¸ ¡ﬂ¿Œ ªÛ≈¬
    vbPath_State_UpDown	= 3,		//¥Ÿ∏• √˛¿∏∑Œ ∫Ø∞Ê ¡ﬂ¿Œ ªÛ≈¬
    vbPath_State_Arrive	= 4,		//∏Ò«• ¡ˆ¡°ø° µµ¬¯«— ªÛ≈¬
    vbPath_State_Pause	= 5		//« ø‰«— ∞ÊøÏ, ¿·Ω√ ∏ÿ√ﬂµµ∑œ «‘(æ∆π´∞Õµµ æ»«‘) - ∏  ∑Œµ˘ µÓ Ω√∞£¿Ã ∞…∏Æ¥¬ ¿€æ˜¿ª «“ ∂ß æ≤∑πµÂø°º≠¿« »£√‚¿ª ¡∂¡§«œ¡ˆ æ ∞Ì ±◊≥… ªÛ≈¬∏¶ ¿Ã∑∏∞‘ «ÿµ÷µµ µ .
} vbPath_State;

//¿ßƒ°∏¶ ≥™≈∏≥ª¥¬ float[3]¥¬ vec3 µÓ¿∏∑Œ ¿˚¿˝»˜ ∫Ø∞Ê«ÿº≠ ªÁøÎ«œ∏È µ .
struct vbPathNode
{
    enum vbPath_Node_Type	type;		//Node¿« ¡æ∑˘ (√˛ ∫Ø∞Ê µÓø° ¬¸¡∂)
    vec3					pos;			//¿ßƒ° - 3¬˜ø¯ ¡¬«•∞Ëø°º≠¿« ¡¬«•
    float					dirDeg;		//¥Ÿ¿Ω Node πÊ«‚(¿⁄µø ∞ËªÍ ∞™¿Ãπ«∑Œ ±◊≥… 0¿ª ≥÷æÓµŒ∏È µ .)
    vec3					dirVec;		//¥Ÿ¿Ω Node πÊ«‚ ∫§≈Õ(¿⁄µø ∞ËªÍ ∞™¿Ãπ«∑Œ ±◊≥… ≥ˆµŒ∏È µ .)
    int						floor;		//√˛ - ¿ßƒ°ø° √˛ ¡§∫∏∞° ∆˜«‘µ«æÓ ¿÷¥ı∂Ûµµ, ∞°±ﬁ¿˚ √˛ ±∏∫–¿ª »ÆΩ«»˜ «œ±‚ ¿ß«ÿ int«¸¿∏∑Œµµ ªÁøÎ
    int						poiId;		//»§Ω√ « ø‰«“¡ˆ ∏∏£¥œ.
};

@protocol MAGuidanceDelegate <NSObject>
@optional
- (void)onGuidanceFinished;
- (void)onGuidancePause;
- (void)onChangedCurPos:(MAVector *)curPos state:(vbPath_State)state;
@end



@interface VBPathGuider : NSObject

@property(nonatomic,assign) id<MAGuidanceDelegate> delegate;

- (id) initWithNaviMode:(vbPath_NaviMode)mode;
- (void) setSpeed:(float)move turn:(float)turn;
- (void) setSpeedAndTurnOffset:(float)move offset:(float)offset;
- (BOOL) setPathNode:(std::vector<vbPathNode> *)newPath;
- (void) initGuide;
- (void) doNextStep;
- (void) doNextStepTurnBezier;
- (void) doNextStepTurnAndGo;
- (void) resolvePauseState;
- (float) getDirDegreeFrom:(vbPathNode)fromNode toNode:(vbPathNode)toNode;
- (float) getDirDegree:(vec3) dirVector;
- (BOOL) isArrivedNextNode:(vec3)pos nextPos:(vec3)nextNodePos;
- (BOOL) isHeadedToNextNode:(float)dirDeg nextDirDeg:(float)nextDirDeg;
- (void) updateTurnSpeedSign:(float)dirDeg nextDirDeg:(float)nextDirDeg;
- (void) updateAheadPosAndOrientation;
- (vec3) getCurPos;
- (float)getCurDirDeg;
@end
