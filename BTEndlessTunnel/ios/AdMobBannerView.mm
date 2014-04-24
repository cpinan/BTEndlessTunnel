//
//  AdMobBannerView.m
//  BTEndlessTunnel
//
//  Created by NSS on 4/8/14.
//
//

#import "AdMobBannerView.h"
#import "Constants.h"
#import "cocos2d.h"

@implementation AdMobBannerView

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
    
    bannerView_ = [[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner];
    bannerView_.adUnitID = [NSString stringWithUTF8String:MY_BANNER_UNIT_ID];
    bannerView_.rootViewController = self;
    bannerView_.adSize = kGADAdSizeSmartBannerLandscape;
    bannerView_.translatesAutoresizingMaskIntoConstraints = YES;
    
    [bannerView_ loadRequest:[GADRequest request]];
    
    [self load];
    /*
    [self.view addConstraint:[NSLayoutConstraint constraintWithItem:bannerView_ attribute:NSLayoutAttributeCenterX relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeCenterX multiplier:1.0 constant:0]];
    */
}

- (void) load
{
    
    [self.view addSubview:bannerView_];

}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    bannerView_.delegate = nil;
    // [bannerView_ release];
    [super dealloc];
}

- (void)hide
{
    if(!self.view.hidden)
    {
        self.view.hidden = YES;
        [bannerView_ removeFromSuperview];
    }
}

- (void)show
{
    if(self.view.hidden)
    {
        self.view.hidden = NO;
        [self load];
    }
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
