package com.hwyncho.mymusicplayer;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.media.MediaMetadataRetriever;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.os.PowerManager;
import android.support.annotation.Nullable;
import android.support.v7.app.NotificationCompat;
import android.widget.RemoteViews;

public class MusicService extends Service {
	protected static final String REQUEST_PLAY = "com.hwyncho.mymusicplayer.service.music.request_play";
	protected static final String REQUEST_PAUSE = "com.hwyncho.mymusicplayer.service.music.request_pause";
	protected static final String REQUEST_STOP = "com.hwyncho.mymusicplayer.service.music.request_stop";
	protected static final String REQUEST_PREV = "com.hwyncho.mymusicplayer.service.music.request_prev";
	protected static final String REQUEST_NEXT = "com.hwyncho.mymusicplayer.service.music.request_next";
	protected static final String REQUEST_SEEK = "com.hwyncho.mymusicplayer.service.music.request_seek";

	protected static final String RESPONSE_INFO = "com.hwyncho.mymusicplayer.service.music.response_info";
	protected static final String RESPONSE_STATE = "com.hwyncho.mymusicplayer.service.music.response_state";

	protected static final String FILEPATH = Environment.getExternalStorageDirectory().getPath() + "/mymusic.mp3";

	private BroadcastReceiver broadcastReceiver;

	private MediaPlayer mediaPlayer;
	private String title;
	private String artist;

	private Runnable task;
	private Thread thread;

	{
		broadcastReceiver = null;

		mediaPlayer = null;
		title = null;
		artist = null;

		task = new Runnable() {
			@Override
			public void run() {
				while (true) {
					if (mediaPlayer.isPlaying()) {
						mySendResponse(RESPONSE_STATE);
						mySetNoti(mediaPlayer.getCurrentPosition());
					}

					try {
						Thread.sleep(500);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
		};
		thread = new Thread(task);
	}

	@Override
	public void onCreate() {
		super.onCreate();

		this.title = getResources().getString(R.string.text_title);
		this.artist = getResources().getString(R.string.text_artist);

		this.mySetPlayer();
		this.mySetReceiver();

		if (this.mediaPlayer != null)
			this.thread.start();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		this.mySendResponse(RESPONSE_INFO);
		this.mySendResponse(RESPONSE_STATE);

		return START_REDELIVER_INTENT;
	}

	@Nullable
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onDestroy() {
		this.myUnsetReceiver();
		this.myUnsetPlayer();

		super.onDestroy();
	}

	private void mySetPlayer() {
		if (this.mediaPlayer == null) {
			this.mediaPlayer = MediaPlayer.create(this, Uri.parse(FILEPATH));
			this.mediaPlayer.setWakeMode(getApplicationContext(), PowerManager.PARTIAL_WAKE_LOCK);
			this.mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
			this.mediaPlayer.setLooping(false);
		}
	}

	private void myUnsetPlayer() {
		if (this.mediaPlayer != null) {
			this.mediaPlayer.release();
			this.mediaPlayer = null;
		}
	}

	private void mySetReceiver() {
		this.broadcastReceiver = new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				if (intent.getAction().equals(REQUEST_PLAY)) {
					mediaPlayer.start();
				} else if (intent.getAction().equals(REQUEST_PAUSE)) {
					mediaPlayer.pause();
					mySendResponse(RESPONSE_STATE);
					mySetNoti(mediaPlayer.getCurrentPosition());
				} else if (intent.getAction().equals(REQUEST_STOP)) {
					mediaPlayer.stop();
					mediaPlayer = MediaPlayer.create(getApplicationContext(), Uri.parse(FILEPATH));
				} else if (intent.getAction().equals(REQUEST_PREV)) {
					if (mediaPlayer.getCurrentPosition() - 10000 > 0)
						mediaPlayer.seekTo(mediaPlayer.getCurrentPosition() - 10000);
					else
						mediaPlayer.seekTo(0);
				} else if (intent.getAction().equals(REQUEST_NEXT)) {
					if (mediaPlayer.getCurrentPosition() + 10000 < mediaPlayer.getDuration())
						mediaPlayer.seekTo(mediaPlayer.getCurrentPosition() + 10000);
					else
						mediaPlayer.seekTo(mediaPlayer.getDuration());
				} else if (intent.getAction().equals(REQUEST_SEEK)) {
					int position = intent.getIntExtra("SEEK", 0);
					mediaPlayer.seekTo(position);
				}
			}
		};

		IntentFilter intentFilter = new IntentFilter();
		intentFilter.addAction(REQUEST_PLAY);
		intentFilter.addAction(REQUEST_PAUSE);
		intentFilter.addAction(REQUEST_STOP);
		intentFilter.addAction(REQUEST_PREV);
		intentFilter.addAction(REQUEST_NEXT);
		intentFilter.addAction(REQUEST_SEEK);

		registerReceiver(this.broadcastReceiver, intentFilter);
	}

	private void myUnsetReceiver() {
		if (this.broadcastReceiver != null) {
			unregisterReceiver(this.broadcastReceiver);
			this.broadcastReceiver = null;
		}
	}

	private void mySendResponse(String request) {
		Bundle bundle = new Bundle();
		Intent broadcast = new Intent();

		if (request.equals(RESPONSE_INFO)) {
			MediaMetadataRetriever metadata = new MediaMetadataRetriever();
			metadata.setDataSource(FILEPATH);

			this.title = metadata.extractMetadata(MediaMetadataRetriever.METADATA_KEY_TITLE);
			this.artist = metadata.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ARTIST);

			bundle.putInt("INFO_MAX", this.mediaPlayer.getDuration());
			bundle.putString("INFO_TITLE", metadata.extractMetadata(MediaMetadataRetriever.METADATA_KEY_TITLE));
			bundle.putString("INFO_ARTIST", metadata.extractMetadata(MediaMetadataRetriever.METADATA_KEY_ARTIST));
			bundle.putString("INFO_END", metadata.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION));

			broadcast.setAction(RESPONSE_INFO);
			broadcast.putExtra("INFO", bundle);
			sendBroadcast(broadcast);
		} else if (request.equals(RESPONSE_STATE)) {
			bundle.putBoolean("STATE_PLAYING", this.mediaPlayer.isPlaying());
			bundle.putInt("STATE_PROGRESS", this.mediaPlayer.getCurrentPosition());

			broadcast.setAction(RESPONSE_STATE);
			broadcast.putExtra("STATE", bundle);
			sendBroadcast(broadcast);
		}
	}

	private void mySetNoti(int progress) {
		Intent intent_activity = new Intent(this, MainActivity.class);
		PendingIntent pendingIntent_activity = PendingIntent.getActivity(this, 0, intent_activity, PendingIntent.FLAG_UPDATE_CURRENT);

		Intent broadcast_prev = new Intent();
		broadcast_prev.setAction(REQUEST_PREV);
		PendingIntent pendingIntent_prev = PendingIntent.getBroadcast(this, 0, broadcast_prev, PendingIntent.FLAG_UPDATE_CURRENT);

		Intent broadcast_play = new Intent();
		broadcast_play.setAction(REQUEST_PLAY);
		PendingIntent pendingIntent_play = PendingIntent.getBroadcast(this, 0, broadcast_play, PendingIntent.FLAG_UPDATE_CURRENT);

		Intent broadcast_pause = new Intent();
		broadcast_pause.setAction(REQUEST_PAUSE);
		PendingIntent pendingIntent_pause = PendingIntent.getBroadcast(this, 0, broadcast_pause, PendingIntent.FLAG_UPDATE_CURRENT);

		Intent broadcast_next = new Intent();
		broadcast_next.setAction(REQUEST_NEXT);
		PendingIntent pendingIntent_next = PendingIntent.getBroadcast(this, 0, broadcast_next, PendingIntent.FLAG_UPDATE_CURRENT);

		RemoteViews remoteViews = new RemoteViews(getPackageName(), R.layout.activity_notification);
		remoteViews.setImageViewResource(R.id.imageView_album, R.drawable.ic_action_av_album);
		remoteViews.setTextViewText(R.id.textView_title, this.title);
		remoteViews.setTextViewText(R.id.textView_artist, this.artist);
		remoteViews.setImageViewResource(R.id.imageButton_prev, R.drawable.ic_action_av_skip_previous);
		if (mediaPlayer.isPlaying())
			remoteViews.setImageViewResource(R.id.imageButton_play, R.drawable.ic_action_av_pause);
		else
			remoteViews.setImageViewResource(R.id.imageButton_play, R.drawable.ic_action_av_play_arrow);
		remoteViews.setImageViewResource(R.id.imageButton_next, R.drawable.ic_action_av_skip_next);
		remoteViews.setProgressBar(R.id.progress_music, mediaPlayer.getDuration(), progress, false);

		remoteViews.setOnClickPendingIntent(R.id.imageView_album, pendingIntent_activity);
		remoteViews.setOnClickPendingIntent(R.id.textView_title, pendingIntent_activity);
		remoteViews.setOnClickPendingIntent(R.id.textView_artist, pendingIntent_activity);
		remoteViews.setOnClickPendingIntent(R.id.imageButton_prev, pendingIntent_prev);
		if (mediaPlayer.isPlaying())
			remoteViews.setOnClickPendingIntent(R.id.imageButton_play, pendingIntent_pause);
		else
			remoteViews.setOnClickPendingIntent(R.id.imageButton_play, pendingIntent_play);
		remoteViews.setOnClickPendingIntent(R.id.imageButton_next, pendingIntent_next);

		NotificationCompat.Builder builder = new NotificationCompat.Builder(this);
		if (mediaPlayer.isPlaying())
			builder.setSmallIcon(R.drawable.ic_stat_av_play_circle_outline);
		else
			builder.setSmallIcon(R.drawable.ic_stat_av_pause_circle_outline);
		builder.setContentTitle(getResources().getString(R.string.app_name));
		builder.setContentText("hwyn.cho");
		builder.setContentIntent(pendingIntent_activity);
		builder.setContentIntent(pendingIntent_prev);
		builder.setContentIntent(pendingIntent_play);
		builder.setContentIntent(pendingIntent_next);
		builder.setCustomContentView(remoteViews);

		NotificationManager manager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
		manager.notify(0, builder.build());
	}
}