//
//  ViewController.m
//  TreeTableView
//
//  Created by yixiang on 15/7/3.
//  Copyright (c) 2015年 yixiang. All rights reserved.
//

#import "ViewController.h"
#import "Node.h"
#import "TreeTableView.h"
#import <objc/runtime.h>
#import "MsgForwardClass.h"
#import "GoodViewController.h"

//void (* functionForThereMethod1)(id self, SEL _cmd);

typedef void (*functionForThereMethod1)(id self, SEL _cmd);

typedef void(^heheda)(BOOL flag);

@interface ViewController ()<TreeTableCellDelegate>
{
    MsgForwardClass *_msgForwardObj;
}
@end

int *pointerMethod(int *a);


void functionForMethod1(id self, SEL _cmd)
{
    NSLog(@"%@, %p", self, _cmd);
}

@implementation ViewController

- (void)haha
{
    NSLog(@"msg_forward");
}

+ (BOOL)resolveInstanceMethod:(SEL)sel //在方法列表中 找不到该 sel, 对该类中添加一个 sel 对该方法进行替换 实现的地址
{
    NSString *selectorString = NSStringFromSelector(sel);
    Method replacedMethod = class_getInstanceMethod([self class], @selector(haha));
    if ([selectorString isEqualToString:@"hehe"])
    {
        if (class_addMethod(self.class, @selector(haha), method_getImplementation(replacedMethod), method_getTypeEncoding(replacedMethod)))
        {
            NSLog(@"New Method Added Successfully! ");
        }
        return YES;
    }
    return [super resolveInstanceMethod:sel];
}

- (id)forwardingTargetForSelector:(SEL)aSelector
{
    if (aSelector == @selector(hehe))
    {
        return _msgForwardObj;
    }
    return [super forwardingTargetForSelector:aSelector];
}

- (NSMethodSignature *)methodSignatureForSelector:(SEL)aSelector
{
    NSMethodSignature *methodSignature = [super methodSignatureForSelector:aSelector];
    if (methodSignature == nil)
    {
       if ([MsgForwardClass instancesRespondToSelector:aSelector])
       {
           methodSignature = [MsgForwardClass instanceMethodSignatureForSelector:aSelector];
       }
    }
    return methodSignature;
}

- (void)forwardInvocation:(NSInvocation *)anInvocation
{
    if ([_msgForwardObj respondsToSelector:anInvocation.selector])
    {
        [anInvocation invokeWithTarget:_msgForwardObj];
        NSInteger result;
        memset(&result, 0, sizeof(result));
        [anInvocation getReturnValue:&result];
        NSLog(@"%ld", result);
        free(&result);
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initData];
   // _msgForwardObj = [MsgForwardClass new];
   // [self performSelector:@selector(hehe) withObject:nil afterDelay:1];
}

-(void)initData{

//----------------------------------中国的省地市关系图3,2,1--------------------------------------------
    Node *country1 = [[Node alloc] initWithParentId:-1 nodeId:0 name:@"中国" depth:0 expand:YES];
    Node *province1 = [[Node alloc] initWithParentId:0 nodeId:1 name:@"江苏" depth:1 expand:NO];
    Node *city1 = [[Node alloc] initWithParentId:1 nodeId:2 name:@"南通" depth:2 expand:NO];
    //Node *subCity1 = [[Node alloc] initWithParentId:2 nodeId:100 name:@"通州" depth:3 expand:NO];
    //Node *subCity2 = [[Node alloc] initWithParentId:2 nodeId:101 name:@"如东" depth:3 expand:NO];
    Node *city2 = [[Node alloc] initWithParentId:1 nodeId:3 name:@"南京" depth:2 expand:NO];
    Node *city3 = [[Node alloc] initWithParentId:1 nodeId:4 name:@"苏州" depth:2 expand:NO];
    Node *province2 = [[Node alloc] initWithParentId:0 nodeId:5 name:@"广东" depth:1 expand:NO];
    Node *city4 = [[Node alloc] initWithParentId:5 nodeId:6 name:@"深圳" depth:2 expand:NO];
    Node *city5 = [[Node alloc] initWithParentId:5 nodeId:7 name:@"广州" depth:2 expand:NO];
    Node *province3 = [[Node alloc] initWithParentId:0 nodeId:8 name:@"浙江" depth:1 expand:NO];
    Node *city6 = [[Node alloc] initWithParentId:8 nodeId:9 name:@"杭州" depth:2 expand:NO];
//----------------------------------美国的省地市关系图0,1,2--------------------------------------------
    Node *country2 = [[Node alloc] initWithParentId:-1 nodeId:10 name:@"美国" depth:0 expand:YES];
    Node *province4 = [[Node alloc] initWithParentId:10 nodeId:11 name:@"纽约州" depth:1 expand:NO];
    Node *province5 = [[Node alloc] initWithParentId:10 nodeId:12 name:@"德州" depth:1 expand:NO];
    Node *city7 = [[Node alloc] initWithParentId:12 nodeId:13 name:@"休斯顿" depth:2 expand:NO];
    Node *province6 = [[Node alloc] initWithParentId:10 nodeId:14 name:@"加州" depth:1 expand:NO];
    Node *city8 = [[Node alloc] initWithParentId:14 nodeId:15 name:@"洛杉矶" depth:2 expand:NO];
    Node *city9 = [[Node alloc] initWithParentId:14 nodeId:16 name:@"旧金山" depth:2 expand:NO];

//----------------------------------日本的省地市关系图0,1,2--------------------------------------------
    Node *country3 = [[Node alloc] initWithParentId:-1 nodeId:17 name:@"日本" depth:0 expand:YES];
    Node *province7 = [[Node alloc] initWithParentId:17 nodeId:18 name:@"东京" depth:1 expand:NO];
    Node *province8 = [[Node alloc] initWithParentId:17 nodeId:19 name:@"东京1" depth:1 expand:NO];
    Node *province9 = [[Node alloc] initWithParentId:17 nodeId:20 name:@"东京2" depth:1 expand:NO];
    
    
    //NSArray *data = [NSArray arrayWithObjects:country1,country2,country3, nil];
    
    //NSArray *data = [NSArray arrayWithObjects:country1,province1,province2,province3,country2,province4,province5,province6,country3, nil];
    
    NSArray *data = [NSArray arrayWithObjects:country1,province1,city1,city2,city3,province2,city4,city5,province3,city6,country2,province4,province5,city7,province6,city8,city9,country3,province7,province8,province9, nil];
    
    
    TreeTableView *tableview = [[TreeTableView alloc] initWithFrame:CGRectMake(0, 0, CGRectGetWidth(self.view.frame), CGRectGetHeight(self.view.frame)-20) withData:data];
    tableview.treeTableCellDelegate = self;
    [self.view addSubview:tableview];
}

#pragma mark - TreeTableCellDelegate
//-(void)cellClick:(Node *)node{
//    NSLog(@"%@",node.name);
//}

- (void)cellClick:(Node *)node didReachToBottom:(BOOL)reached
{
    if (reached)
    {
        GoodViewController *good = [GoodViewController new];
        [self.navigationController pushViewController:good animated:YES];
        NSLog(@"到了叶子节点, 这里写去 push 的操作");
    }
    else
    {
        NSLog(@"其他操作");
    }
}

@end
