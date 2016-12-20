package com.hwyncho.mymusicplayer;

import android.Manifest;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
	protected static final String REQUEST_PLAY = "com.hwyncho.service.music.request_play";
	protected static final String REQUEST_PAUSE = "com.hwyncho.service.music.request_pause";
	protected static final String REQUEST_STOP = "com.hwyncho.service.music.request_stop";
	protected static final String REQUEST_PREV = "com.hwyncho.service.music.request_prev";
	protected static final String REQUEST_NEXT = "com.hwyncho.service.music.request_next";
	protected static final String REQUEST_SEEK = "com.hwyncho.service.music.request_seek";

	protected static final String RESPONSE_INFO = "com.hwyncho.service.music.response_info";
	protected static final String RESPONSE_STATE = "com.hwyncho.service.music.response_state";

	private static final int MY_PERMISSION_REQUEST_STORAGE = 0;

	private BroadcastReceiver broadcastReceiver;

	private SeekBar seekBar;

	private String title;
	private String artist;
	private boolean isPlaying;

	{
		broadcastReceiver = null;

		seekBar = null;

		title = null;
		artist = null;
		isPlaying = false;
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		this.myFindView();
		this.mySetReceiver();

		this.myCheckPermission();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

		this.myUnsetReceiver();
	}

	@Override
	public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
		//super.onRequestPermissionsResult(requestCode, permissions, grantResults);

		switch (requestCode) {
			case MY_PERMISSION_REQUEST_STORAGE:
				if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
					startService(new Intent(this, MusicService.class));
				} else {
					finish();
				}
				break;
		}
	}

	private void myCheckPermission() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
			if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
				requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, MY_PERMISSION_REQUEST_STORAGE);
			} else {
				startService(new Intent(this, MusicService.class));
			}
		} else {
			startService(new Intent(this, MusicService.class));
		}
	}

	private void myFindView() {
		DisplayMetrics displayMetrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

		int displayWidth = displayMetrics.widthPixels;
		int displayHeight = displayMetrics.heightPixels;

		ImageView imageView = (ImageView) findViewById(R.id.imageView_album);
		imageView.getLayoutParams().width = (int) (displayWidth * 0.8);
		imageView.getLayoutParams().height = (int) (displayWidth * 0.8);

		this.title = getResources().getString(R.string.text_title);
		this.artist = getResources().getString(R.string.text_artist);

		seekBar = (SeekBar) findViewById(R.id.seekBar_music);
		seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				if (fromUser == true) {
					Intent broadcast = new Intent();
					broadcast.setAction(REQUEST_SEEK);
					broadcast.putExtra("SEEK", progress);
					sendBroadcast(broadcast);
				}
			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {

			}

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {

			}
		});

		ImageButton imageButton_play = (ImageButton) findViewById(R.id.imageButton_play);
		imageButton_play.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				if (!isPlaying) {
					mySetState(true);
					mySendRequest(REQUEST_PLAY);
				} else {
					mySetState(false);
					mySendRequest(REQUEST_PAUSE);
				}
			}
		});

		ImageButton imageButton_stop = (ImageButton) findViewById(R.id.imageButton_stop);
		imageButton_stop.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				mySetState(false);
				mySendRequest(REQUEST_STOP);
			}
		});

		ImageButton imageButton_prev = (ImageButton) findViewById(R.id.imageButton_prev);
		imageButton_prev.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				mySendRequest(REQUEST_PREV);
			}
		});

		ImageButton imageButton_next = (ImageButton) findViewById(R.id.imageButton_next);
		imageButton_next.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				mySendRequest(REQUEST_NEXT);
			}
		});
	}

	private void mySetReceiver() {
		this.broadcastReceiver = new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				if (intent.getAction().equals(RESPONSE_INFO)) {
					Bundle bundle = intent.getBundleExtra("INFO");
					seekBar.setMax(bundle.getInt("INFO_MAX", 0));
					((TextView) findViewById(R.id.textView_title)).setText(bundle.getString("INFO_TITLE", title));
					((TextView) findViewById(R.id.textView_artist)).setText(bundle.getString("INFO_ARTIST", artist));
					((TextView) findViewById(R.id.textView_end)).setText(myConvert(bundle.getString("INFO_END", "1:11")));
				} else if (intent.getAction().equals(RESPONSE_STATE)) {
					Bundle bundle = intent.getBundleExtra("STATE");
					mySetState(bundle.getBoolean("STATE_PLAYING", false));
					int progress = bundle.getInt("STATE_PROGRESS", 0);
					seekBar.setProgress(progress);
					((TextView) findViewById(R.id.textView_start)).setText(myConvert(String.valueOf(progress)));
				}
			}
		};

		IntentFilter intentFilter = new IntentFilter();
		intentFilter.addAction(RESPONSE_INFO);
		intentFilter.addAction(RESPONSE_STATE);

		registerReceiver(this.broadcastReceiver, intentFilter);
	}

	private void myUnsetReceiver() {
		if (this.broadcastReceiver != null) {
			unregisterReceiver(this.broadcastReceiver);
			this.broadcastReceiver = null;
		}
	}

	private void mySetState(boolean isPlaying) {
		this.isPlaying = isPlaying;

		ImageButton imageButton_play = (ImageButton) findViewById(R.id.imageButton_play);
		if (isPlaying == true) {
			imageButton_play.setImageResource(R.drawable.ic_av_pause);
		} else {
			imageButton_play.setImageResource(R.drawable.ic_av_play_arrow);
		}
	}

	private void mySendRequest(String request) {
		Intent broadcast = new Intent();
		broadcast.setAction(request);
		sendBroadcast(broadcast);
	}

	private String myConvert(String duration) {
		long millisecond = Long.parseLong(duration);
		long second = millisecond / 1000;
		long hour = second / 3600;
		long minute = (second - (3600 * hour)) / 60;
		long second2 = second - (3600 * hour + 60 * minute);

		String result = "";

		if (hour > 0) {
			result += String.valueOf(hour);
			result += ":";
		}

		result += String.valueOf(minute);
		result += ":";

		if (second2 < 10)
			result += "0";

		result += String.valueOf(second2);

		return result;
	}
}