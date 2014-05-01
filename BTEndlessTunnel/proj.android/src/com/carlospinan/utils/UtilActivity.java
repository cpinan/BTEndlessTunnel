package com.carlospinan.utils;

import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.carlospinan.turborace.R;
import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.facebook.widget.FacebookDialog;
import com.facebook.widget.WebDialog;
import com.facebook.widget.WebDialog.OnCompleteListener;
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
	private FrameLayout adViewLayout = null;
	public static final String TAG = "UtilActivity";
	private UiLifecycleHelper uiHelper;

	private Session.StatusCallback statusCallback = new Session.StatusCallback() {
		@Override
		public void call(Session session, SessionState state,
				Exception exception) {
			onSessionStateChange(session, state, exception);
		}
	};

	/**
	 * 
	 */
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		_init();

		uiHelper = new UiLifecycleHelper(this, statusCallback);
		uiHelper.onCreate(savedInstanceState);
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

		// setContentView(R.layout.admob);

		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
				FrameLayout.LayoutParams.MATCH_PARENT,
				FrameLayout.LayoutParams.WRAP_CONTENT);
		params.gravity = Gravity.TOP | Gravity.CENTER_HORIZONTAL;

		LayoutInflater inflate = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		View view = inflate.inflate(R.layout.admob, null);
		addContentView(view, params);

		adViewLayout = (FrameLayout) view.findViewById(R.id.admob);

		// Create an ad.
		adView = new AdView(this);
		adView.setAdSize(AdSize.BANNER);
		adView.setAdUnitId(getResources().getString(R.string.AD_UNIT_ID));
		adView.setLayoutParams(params);

		// Add the AdView to the view hierarchy. The view will have no size
		// until the ad is loaded.
		adViewLayout.addView(adView);

		// Create an ad request. Check logcat output for the hashed
		// device ID to
		// get test ads on a physical device.
		AdRequest adRequest = new AdRequest.Builder()
				.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
				.addTestDevice("INSERT_YOUR_HASHED_DEVICE_ID_HERE").build();

		// Start loading the ad in the background.
		adView.loadAd(adRequest);

		adViewLayout.setBackgroundColor(Color.BLACK);
		adViewLayout.setBackgroundColor(0);
		showAd();
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
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adViewLayout.setVisibility(View.VISIBLE);
			adView.resume();
		}
	}

	public void hideAd() {
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adViewLayout.setVisibility(View.GONE);
			adView.pause();
		}
	}

	@Override
	protected void onPause() {
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adView.pause();
		}
		super.onPause();
		uiHelper.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adView.resume();
		}

		uiHelper.onResume();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
		uiHelper.onSaveInstanceState(outState);
	}

	@Override
	protected void onDestroy() {
		if (ConfigUtils.USE_AD_MOB && adView != null) {
			adView.destroy();
		}
		super.onDestroy();
		uiHelper.onDestroy();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		uiHelper.onActivityResult(requestCode, resultCode, data,
				new FacebookDialog.Callback() {
					@Override
					public void onError(FacebookDialog.PendingCall pendingCall,
							Exception error, Bundle data) {
						Log.e("Activity",
								String.format("Error: %s", error.toString()));
					}

					@Override
					public void onComplete(
							FacebookDialog.PendingCall pendingCall, Bundle data) {
						Log.i("Activity", "Success!");
					}
				});
	}

	public void shareOnFacebook(int level, long score, int obstacles) {

		String modo = getResources().getString(R.string.easy);
		if (level == 1)
			modo = getResources().getString(R.string.normal);
		else if (level == 2)
			modo = getResources().getString(R.string.hard);

		String name = "Turbo Race";
		String caption = getResources().getString(R.string.caption);
		String picture = "http://carlospinan.com/icon/turbo_race.png";

		String description = getResources().getString(R.string.fb_description);
		description = description.replace("{score}", score + "");
		description = description.replace("{modo}", modo + "");
		description = description.replace("{obstaculos}", obstacles + "");

		String link = "https://play.google.com/store/apps/details?id=com.carlospinan.turborace";

		if (FacebookDialog.canPresentShareDialog(getApplicationContext(),
				FacebookDialog.ShareDialogFeature.SHARE_DIALOG)) {
			// Publish the post using the Share Dialog
			FacebookDialog shareDialog = new FacebookDialog.ShareDialogBuilder(
					this).setLink(link).setName(name).setCaption(caption)
					.setPicture(picture).setDescription(description).build();
			uiHelper.trackPendingDialogCall(shareDialog.present());

		} else {
			// Fallback. For example, publish the post using the Feed Dialog
			publishFeedDialog(name, caption, description, link, picture);
		}

	}

	public void publishFeedDialog(String name, String caption,
			String description, String link, String picture) {

		final Context context = UtilActivity.this;
		final Session session = Session.getActiveSession();
		if (session != null && session.isOpened()) {

			Bundle params = new Bundle();
			params.putString("name", name);
			params.putString("caption", caption);
			params.putString("description", description);
			params.putString("link", link);
			params.putString("picture", picture);

			WebDialog feedDialog = (new WebDialog.FeedDialogBuilder(context,
					Session.getActiveSession(), params)).setOnCompleteListener(
					new OnCompleteListener() {

						@Override
						public void onComplete(Bundle values,
								FacebookException error) {
							if (error == null) {
								// When the story is posted, echo the success
								// and the post Id.
								final String postId = values
										.getString("post_id");
								if (postId != null) {
									Toast.makeText(context,
											"Posted story, id: " + postId,
											Toast.LENGTH_SHORT).show();
								} else {
									// User clicked the Cancel button
									Toast.makeText(context,
											"Publish cancelled",
											Toast.LENGTH_SHORT).show();
								}
							} else if (error instanceof FacebookOperationCanceledException) {
								// User clicked the "x" button
								Toast.makeText(context, "Publish cancelled",
										Toast.LENGTH_SHORT).show();
							} else {
								// Generic, ex: network error
								Toast.makeText(context, "Error posting story",
										Toast.LENGTH_SHORT).show();
							}
						}

					}).build();
			feedDialog.show();

		} else {

			AlertDialog.Builder builder = new AlertDialog.Builder(context);
			builder.setMessage("Facebook connection error.");
			builder.setNeutralButton(
					context.getResources().getString(android.R.string.ok), null);
			builder.create().show();

		}

	}

	private void onSessionStateChange(Session session, SessionState state,
			Exception exception) {
		if (state.isOpened()) {
		} else if (state.isClosed()) {
		}
	}

	/*
	 * Cocos2d-x Library
	 */
	static {
		System.loadLibrary("game");
	}

}
