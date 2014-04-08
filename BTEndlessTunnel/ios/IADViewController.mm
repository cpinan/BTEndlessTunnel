//
//  IADViewController.m
//  BTEndlessTunnel
//
//  Created by NSS on 4/8/14.
//
//

#import "IADViewController.h"
#import "cocos2d.h"

@interface IADViewController ()

@end

@implementation IADViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    _bannerView = [[ADBannerView alloc] initWithFrame:CGRectZero];
    _bannerView.backgroundColor = [UIColor clearColor];
    
    float width = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width;
    float height = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().height;
    
    [_bannerView setFrame:CGRectMake(0, 0, width, height)];

    _bannerView.delegate = self;
    [self.view addSubview:_bannerView];
    // Do any additional setup after loading the view.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)hide
{
    [_bannerView removeFromSuperview];
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
