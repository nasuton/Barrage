#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "cocos2d.h"

class SoundManager {
private:
	enum AudioType {
		BGM = 0,
		SE,
	};

	enum FadeType {
		NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_IN_RESUME,
		FADE_OUT_PAUSE,
	};

	static SoundManager* instance;

	static cocos2d::Scheduler* managerSceheduler;

	//管理音源(jsonで管理する際に使用)
	std::map<std::string, std::string> bgmList;
	std::map<std::string, std::string> seList;

	//SEをいくつ使用するか
	int seChunk[4];

	//BGMのID
	int bgmID;

	//現在流しているものと同じかどうか判断するため
	std::string bgmFileName;

	//拡張子判定用
	std::string bgmFileExt;

	//ゲッターとセッター
	CC_SYNTHESIZE(std::string, audioListFile, AudioFile);

	//音量
	float bgmVolume;
	float seVolume;

	//BGMフェード関連
	FadeType fadeCondition;
	float fadeVolumeFrom;
	float fadeVolumeTo;
	float fadeVolumeNow;
	float fadeTime;
	bool stopBGMreleaseFlg;

public:
	~SoundManager();

	static SoundManager* getInstance();

	static void DeleteInstance();

	virtual void update(float _dt);

	//管理ファイルを読み込む
	bool ReadAudioFile(std::string _jsonFileName);

	//すべてを解放する
	void ReleaseAll();

	//音量を初期化
	void InitVolume(float _bgm, float _se);

	//AudioEngine解放
	void EndAudioEngine();

	//------------------------------------ BGM ------------------------------------//
	//BGM再生
	int PlayBGM(std::string _bgmName, float _fade = 0.0f, bool _loop = true);
	int PlayBGM(std::string _bgmName, float _fade, bool _loop, float _volume);

	//BGM一時停止
	void PauseBGM(float _fade = 0.0f);


	//BGM停止
	void StopBGM(float _fade = 0.0f, bool _release = true);

	//BGM再生中かどうか
	bool IsPlayingBGM();

	//BGM音量変更
	void SetBGMVolume(float _volume, bool _save = true);

	//BGM音量取得
	float GetBGMVolume();

	//BGMキャッシュ解放
	void ReleaseBGM();

	//------------------------------------ SE ------------------------------------//
	//SE再生
	int PlaySE(std::string _seName, int _chunkNumber);
	int PlaySE(std::string _seName, int _chunkNumber, bool _loop, float _volume);
	int PlaySE(std::string _seName, bool _loop = false);
	int PlaySE(std::string _seName, bool _loop, float _volume);

	//SEの停止
	void StopSE(int _seId);

	//SEの音量変更
	void SetSEVolume(float _volume);

	//SE音量取得
	float GetSEVolume();

	//SEキャッシュ解放
	void ReleaseSE(std::string _seName);

private:
	SoundManager();

	//拡張子の取得
	std::string GetExtension(AudioType _type, std::string _audioName);

	//pauseBGMの実行
	void PauseBGMEngine();

	//stopBGM
	void StopBGMEngine(bool _release = true);
};

#endif