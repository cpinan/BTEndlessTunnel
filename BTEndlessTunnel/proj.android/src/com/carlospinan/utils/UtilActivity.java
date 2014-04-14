package com.carlospinan.utils;

import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.carlospinan.turborace.R;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.appstate.AppStateManager;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.basegameutils.BaseGameActivity;

/**
 * 
 * @author Carlos Eduardo Piñan Indacochea
 * @version 1.0.0
 * @date 2014/02/25
 * @update 2014/02/25
 * 
 */
public class UtilActivity extends BaseGameActivity {

	private AdView adView = null;
	private LinearLayout adViewLayout = null;
	public static final String TAG = "UtilActivity";

	/**
	 * 
	 */
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		_init();
	}

	/**
	 * 
	 */
	private void _init() {
		NativeUtils.configure(this);
		if (ConfigUtils.USE_AD_MOB) {
			_initAdMob();
		}
	}

	private void _initAdMob() {

		LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
				LinearLayout.LayoutParams.MATCH_PARENT,
				LinearLayout.LayoutParams.WRAP_CONTENT);
		params.gravity = Gravity.TOP | Gravity.CENTER_HORIZONTAL;

		// Create an ad.
		adView = new AdView(this);
		adView.setAdSize(AdSize.BANNER);
		adView.setAdUnitId(getResources().getString(R.string.AD_UNIT_ID));
		adView.setLayoutParams(params);

		// Add the AdView to the view hierarchy. The view will have no size
		// until the ad is loaded.

		adViewLayout = new LinearLayout(this);
		adViewLayout.setLayoutParams(params);
		adViewLayout.addView(adView);

		if (ConfigUtils.AD_MOB_DEBUG) {
			// Create an ad request. Check logcat output for the hashed
			// device ID to
			// get test ads on a physical device.
			AdRequest adRequest = new AdRequest.Builder()
					.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
					.addTestDevice("INSERT_YOUR_HASHED_DEVICE_ID_HERE").build();

			// Start loading the ad in the background.
			adView.loadAd(adRequest);
		}

		this.addContentView(adViewLayout, params);
	}

	@Override
	public void onSignInFailed() {
		// Override in child class
	}

	@Override
	public void onSignInSucceeded() {
		// Override in child class
	}

	/**
	 * Get boolean to identify if user is signed in
	 * 
	 * @return
	 */
	public boolean getSignedIn() {
		return isSignedIn();
	}

	/**
	 * 
	 * @return
	 */
	public GoogleApiClient getCustomApiClient() {
		return getApiClient();
	}

	/**
	 * 
	 */
	public void signInGooglePlay() {
		beginUserInitiatedSignIn();
	}

	/**
	 * 
	 */
	public void signOutGooglePlay() {
		signOut();
	}

	// InCloud methods not work yet.

	/**
	 * 
	 * @param key
	 * @param app_state
	 */
	public void inCloudSaveOrUpdate(final int key, final byte[] app_state) {
		AppStateManager.update(getCustomApiClient(), key, app_state);
	}

	/**
	 * 
	 * @param key
	 */
	public void inCloudLoad(final int key) {
		AppStateManager.load(getCustomApiClient(), key);
	}

	/**
	 * 
	 */
	public void requestGameAndCloudSave() {
		setRequestedClients(BaseGameActivity.CLIENT_GAMES
				| BaseGameActivity.CLIENT_APPSTATE);
	}

	public void showAd() {
		if (ConfigUtils.USE_AD_MOB && adView != null)
			adViewLayout.setVisibility(View.VISIBLE);
	}

	public void hideAd() {
		if (ConfigUtils.USE_AD_MOB && adView != null)
			adViewLayout.setVisibility(View.GONE);
	}

	@Override
	protected void onPause() {
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adView.pause();
		}
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adView.resume();
		}
	}

	@Override
	protected void onDestroy() {
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adView.destroy();
		}
		super.onDestroy();
	}

	/*
	 * Cocos2d-x Library
	 */
	static {
		System.loadLibrary("game");
	}

}
