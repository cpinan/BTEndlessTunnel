//
//  IADViewController.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/8/14.
//
//

#import <UIKit/UIKit.h>
#import <iAd/iAd.h>

@interface IADViewController : UIViewController<ADBannerViewDelegate>
{
    
}

@property (strong, nonatomic) ADBannerView* bannerView;

-(void) hide;

@end
