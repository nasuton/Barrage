#include "SoundManager.h"
#include "audio/include/AudioEngine.h"
#include "System/JsonHelper/JsonHelper.h"

USING_NS_CC;
using namespace experimental;

SoundManager* SoundManager::instance = nullptr;
Scheduler* SoundManager::managerSceheduler = nullptr;

SoundManager::SoundManager() 
:bgmID(-1)
,bgmFileName("")
,bgmFileExt("")
,bgmVolume(0.5f)
,seVolume(0.5f)
,fadeCondition(FadeType::NONE)
,fadeVolumeFrom(0.0f)
,fadeVolumeTo(0.0f)
,fadeVolumeNow(0.0f)
,fadeTime(0.0f)
,stopBGMreleaseFlg(true) 
{
	for (int i = 0; i < sizeof(seChunk) / sizeof(seChunk[0]); i++) {
		seChunk[i] = AudioEngine::INVALID_AUDIO_ID;
	}
}

SoundManager::~SoundManager() {
	CC_SAFE_RELEASE_NULL(managerSceheduler);
}

SoundManager* SoundManager::getInstance() {
	if (instance == nullptr) {
		instance = new SoundManager();

		managerSceheduler = Director::getInstance()->getScheduler();
		managerSceheduler->retain();
		managerSceheduler->scheduleUpdate(instance, 0, false);
	}
	
	return instance;
}

void SoundManager::DeleteInstance() {
	if (instance != nullptr) {
		delete instance;
	}

	instance = nullptr;
}

void SoundManager::update(float _dt) {
	//fadeOut,fadeInするために使用
	switch (fadeCondition) {
	case FadeType::FADE_IN:
	case FadeType::FADE_IN_RESUME:
		//fadeTimeが0以下の場合は、0.1に設定する
		if (fadeTime <= 0.0f) {
			fadeTime = 0.1f;
		}

		fadeVolumeNow += (_dt * (fadeVolumeTo - fadeVolumeFrom)) / fadeTime;

		if (fadeVolumeTo <= fadeVolumeNow) {
			fadeVolumeNow = fadeVolumeTo;
			fadeVolumeFrom = fadeVolumeTo;
			fadeCondition = FadeType::NONE;
		}

		this->SetBGMVolume(fadeVolumeNow, false);

		break;

	case FadeType::FADE_OUT:
	case FadeType::FADE_OUT_PAUSE:
		//fadeTimeが0以下の場合は、0.1に設定する
		if (fadeTime <= 0.0f) {
			fadeTime = 0.1f;
		}

		fadeVolumeNow += (_dt * (fadeVolumeTo - fadeVolumeFrom)) / fadeTime;

		if (fadeVolumeNow <= fadeVolumeTo) {
			fadeVolumeNow = fadeVolumeTo;
			fadeVolumeFrom = fadeVolumeTo;

			if (fadeCondition == FadeType::FADE_OUT) {
				this->StopBGM(0.0f, stopBGMreleaseFlg);
			}
			else if (fadeCondition == FadeType::FADE_OUT_PAUSE) {
				this->PauseBGM(0.0f);
			}

			fadeCondition = FadeType::NONE;
		}
		this->SetBGMVolume(fadeVolumeNow, false);

		break;

	default:
		break;
	}
}

//jsonを使用して管理する際に使う
bool SoundManager::ReadAudioFile(std::string _jsonFileName) {

	//一度読み込まれていたら処理をしない
	if (audioListFile == _jsonFileName) {
		log("%s has been already read once", _jsonFileName.c_str());
		return false;
	}

	//jsonパースでエラーが起きた場合
	rapidjson::Document audioDoc = JsonHelper::DocumentParse<rapidjson::kParseDefaultFlags>(_jsonFileName);
	if (audioDoc.HasParseError()) {
		log("json parse error : %s.", _jsonFileName.c_str());
		return false;
	}

	if (audioDoc.IsObject()) {
		bgmList.clear();
		seList.clear();

		const rapidjson::Value& listBGM = audioDoc["BGM"];
		for (rapidjson::Value::ConstMemberIterator itr = listBGM.MemberBegin(); itr != listBGM.MemberEnd(); itr++) {
			std::string key = itr->name.GetString();
			const rapidjson::Value& value = itr->value;
			if (value.GetType() == rapidjson::kStringType) {
				bgmList[key] = value.GetString();
			}
		}

		const rapidjson::Value& listSE = audioDoc["SE"];
		for (rapidjson::Value::ConstMemberIterator itr = listSE.MemberBegin(); itr != listSE.MemberEnd(); itr++) {
			std::string key = itr->name.GetString();
			const rapidjson::Value& value = itr->value;
			if (value.GetType() == rapidjson::kStringType) {
				seList[key] = value.GetString();
			}
		}

		audioListFile = _jsonFileName;
		return true;
	}

	return false;
}

void SoundManager::ReleaseAll() {
	AudioEngine::uncacheAll();
}

void SoundManager::InitVolume(float _bgm, float _se) {
	bgmVolume = _bgm;
	seVolume = _se;
}

void SoundManager::EndAudioEngine() {
	AudioEngine::end();
}

int SoundManager::PlayBGM(std::string _bgmName, float _fade, bool _loop) {
	return this->PlayBGM(_bgmName, _fade, _loop, bgmVolume);
}

int SoundManager::PlayBGM(std::string _bgmName, float _fade, bool _loop, float _volume) {
	std::string fileName = this->GetExtension(AudioType::BGM, _bgmName);

	if (fileName == "") {
		return AudioEngine::INVALID_AUDIO_ID;
	}

	//すでに同じものが流れていた場合、何もしないで返す
	if (bgmFileName == _bgmName && AudioEngine::getState(bgmID) == AudioEngine::AudioState::PLAYING) {
		return bgmID;
	}

	bgmFileExt = fileName.substr(fileName.size() - 4, 4);

	this->StopBGM();

	if (0.0f <= _fade) {
		fadeCondition = FadeType::FADE_IN;
		fadeVolumeNow = 0.0f;
		fadeVolumeFrom = 0.0f;
		fadeTime = _fade;
	}
	else {
		fadeCondition = FadeType::NONE;
		fadeVolumeNow = _volume;
	}

	fadeVolumeTo = _volume;

	bgmID = AudioEngine::play2d(fileName, _loop, bgmVolume);

	//ループの設定
	if (_loop) {
		AudioEngine::setFinishCallback(bgmID,
			[this, _loop, _volume](int _audioID, std::string _file) {
			this->StopBGM(0.0f, false);
			bgmID = PlayBGM(bgmFileName, 0.0f, _loop, _volume);
		});
	}

	bgmFileName = _bgmName;

	return bgmID;
}

void SoundManager::PauseBGM(float _fade) {
	fadeVolumeTo = 0.0f;
	if (0.0f <= _fade) {
		fadeCondition = FadeType::FADE_OUT_PAUSE;
		fadeVolumeNow = bgmVolume;
		fadeVolumeFrom = bgmVolume;
		fadeTime = _fade;
	}
	else {
		fadeCondition = FadeType::NONE;
		fadeVolumeNow = 0.0f;

		this->PauseBGMEngine();
	}
}

void SoundManager::StopBGM(float _fade, bool _release) {
	fadeVolumeTo = 0.0f;

	if (0.0f <= _fade) {
		fadeCondition = FadeType::FADE_OUT;
		fadeVolumeNow = bgmVolume;
		fadeVolumeFrom = bgmVolume;
		fadeTime = _fade;
		stopBGMreleaseFlg = _release;
	}
	else {
		fadeCondition = FadeType::NONE;
		fadeVolumeNow = 0.0f;
		this->StopBGMEngine(_release);
	}
}

bool SoundManager::IsPlayingBGM() {
	if (bgmFileName == "") {
		return false;
	}

	std::string fileName = bgmFileName + bgmFileExt;

	AudioEngine::AudioState status = AudioEngine::getState(bgmID);
	if (status == AudioEngine::AudioState::PLAYING) {
		return true;
	}

	return false;
}

void SoundManager::SetBGMVolume(float _volume, bool _save) {
	if (_save) {
		bgmVolume = _volume;
	}

	AudioEngine::setVolume(bgmID, _volume);
}

float SoundManager::GetBGMVolume() {
	return bgmVolume;
}

void SoundManager::ReleaseBGM() {
	std::string fileName = bgmFileName + bgmFileExt;
	AudioEngine::uncache(fileName);
}

int SoundManager::PlaySE(std::string _seName, int _chunkNumber) {
	return this->PlaySE(_seName, _chunkNumber, false, seVolume);
}

int SoundManager::PlaySE(std::string _seName, int _chunkNumber, bool _loop, float _volume) {
	int soundID = AudioEngine::INVALID_AUDIO_ID;

	bool chunkFlg = false;

	std::string fileName = GetExtension(AudioType::SE, _seName);

	if (fileName == "") {
		return soundID;
	}

	if (0 <= _chunkNumber && _chunkNumber < (sizeof(seChunk) / sizeof(seChunk[0]))) {
		chunkFlg = true;

		this->StopSE(seChunk[_chunkNumber]);
	}

	soundID = AudioEngine::play2d(fileName, _loop, _volume);

	if (chunkFlg) {
		seChunk[_chunkNumber] = soundID;
	}

	return soundID;
}

int SoundManager::PlaySE(std::string _seName, bool _loop) {
	return this->PlaySE(_seName, _loop, seVolume);
}

int SoundManager::PlaySE(std::string _seName, bool _loop, float _volume) {
	return this->PlaySE(_seName, -1, _loop, _volume);
}

void SoundManager::StopSE(int _seId) {
	AudioEngine::stop(_seId);
}

void SoundManager::SetSEVolume(float _volume) {
	seVolume = _volume;
}

float SoundManager::GetSEVolume() {
	return seVolume;
}

void SoundManager::ReleaseSE(std::string _seName) {
	std::string fileName = this->GetExtension(AudioType::SE, _seName);

	if (fileName == "") {
		return;
	}

	AudioEngine::uncache(fileName);
}

std::string SoundManager::GetExtension(AudioType _type, std::string _audioName) {
	
	if (_audioName == "") {
		log("sound name is none.");
		return "";
	}

	ApplicationProtocol::Platform platform = Application::getInstance()->getTargetPlatform();

	//拡張子
	std::string extension = ".m4a";

	//管理ファイルを使う場合、キーからファイル名を取得する
	if (audioListFile != "") {
		if (_type == AudioType::BGM) {
			if (bgmList.count(_audioName) != 0) {
				_audioName = bgmList[_audioName];
			}
		}
		else if (_type == AudioType::SE) {
			if (seList.count(_audioName) != 0) {
				_audioName = seList[_audioName];
			}
		}
	}

	//すでに拡張子が含まれているならそのまま返す
	if (_audioName.find_last_of(".") != std::string::npos) {
		if (FileUtils::getInstance()->isFileExist(_audioName)) {
			return _audioName;
		}
		else {
			return "";
		}
	}

	switch (platform)
	{
	//ここに対応するプラットフォームを追加していく
	case ApplicationProtocol::Platform::OS_WINDOWS:
		if (_type == AudioType::BGM) {
			//ここに探す拡張子の優先順位ごとに書いていく
		}
		else if (_type == AudioType::SE) {
			//ここに探す拡張子の優先順位ごとに書いていく
		}
		break;
	default:
		break;
	}

	if (FileUtils::getInstance()->isFileExist(_audioName + extension)) {
		return _audioName + extension;
	}


	log("file not found %s.", _audioName.c_str());
	return _audioName;
}

void SoundManager::PauseBGMEngine() {
	AudioEngine::pause(bgmID);
}

void SoundManager::StopBGMEngine(bool _release) {
	AudioEngine::stop(bgmID);

	if (_release) {
		this->ReleaseBGM();
	}

	bgmID = AudioEngine::INVALID_AUDIO_ID;
	bgmFileName = "";
	bgmFileExt = "";
}