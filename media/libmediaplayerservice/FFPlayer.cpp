/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "FFPlayer"
#include <utils/Log.h>
#include "FFPlayer.h"
#include "rkffplayer.h"
#include <media/Metadata.h>
#include <media/stagefright/MediaExtractor.h>
//#undef ALOGV
//#define ALOGV ALOGE
namespace android {

FFPlayer::FFPlayer()
    : mPlayer(new FF_MediaPlayer) {
    ALOGV("FFPlayer");
    mPlayer->setListener(this);
}

FFPlayer::~FFPlayer() {

    ALOGE("~FFPlayer");
    reset();
    delete mPlayer;
    mPlayer = NULL;

}

status_t FFPlayer::initCheck() {
    ALOGV("initCheck");
    return OK;
}

status_t FFPlayer::setUID(uid_t uid) {
  //  mPlayer->setUID(uid);

    return OK;
}

status_t FFPlayer::setDataSource(
        const char *url, const KeyedVector<String8, String8> *headers) {

    return mPlayer->setDataSource(url, headers);
}

// Warning: The filedescriptor passed into this method will only be valid until
// the method returns, if you want to keep it, dup it!
status_t FFPlayer::setDataSource(int fd, int64_t offset, int64_t length) {
    ALOGV("setDataSource(%d, %lld, %lld)", fd, offset, length);
    return mPlayer->setDataSource(fd, offset, length);
}

status_t FFPlayer::setDataSource(const sp<IStreamSource> &source) {
    return OK;
}

#ifdef AVS44
status_t FFPlayer::setVideoSurfaceTexture(
        const sp<IGraphicBufferProducer> &bufferProducer) {
    ALOGV("setVideoSurfaceTexture");
    return mPlayer->setSurfaceTexture(bufferProducer);
}
#else
status_t FFPlayer::setVideoSurfaceTexture(
        const sp<ISurfaceTexture> &surfaceTexture) {
    ALOGV("setVideoSurfaceTexture");

    return mPlayer->setSurfaceTexture(surfaceTexture);
}
#endif

status_t FFPlayer::prepare() {

    ALOGV("prepare");
    return mPlayer->prepare();
}

status_t FFPlayer::prepareAsync() {

    ALOGV("prepareAsync");
    return mPlayer->prepareAsync();
}

status_t FFPlayer::start() {
    ALOGV("start");
    return mPlayer->start();
}

status_t FFPlayer::stop() {
    ALOGV("stop");
    return pause();  // what's the difference?
}

status_t FFPlayer::pause() {
    ALOGV("pause");

    return mPlayer->pause();
}

bool FFPlayer::isPlaying() {
    ALOGV("isPlaying");
    return mPlayer->isPlaying();
}

status_t FFPlayer::seekTo(int msec) {
    ALOGV("seekTo %.2f secs", msec / 1E3);

    status_t err = mPlayer->seekTo((int64_t)msec * 1000);

    return err;
}

status_t FFPlayer::getCurrentPosition(int *msec) {
    ALOGV("getCurrentPosition");

    int64_t positionUs;
    status_t err = mPlayer->getPosition(&positionUs);

    if (err != OK) {
        return err;
    }

    *msec = (positionUs + 500) / 1000;

    return OK;
}

status_t FFPlayer::getDuration(int *msec) {
    ALOGV("getDuration");

    int64_t durationUs;
    status_t err = mPlayer->getDuration(&durationUs);

    if (err != OK) {
        *msec = 0;
        return OK;
    }

    *msec = (durationUs + 500) / 1000;

    return OK;
}

status_t FFPlayer::reset() {
    ALOGV("reset");

    mPlayer->reset();

    return OK;
}

status_t FFPlayer::setLooping(int loop) {
    ALOGV("setLooping");
    return OK;// mPlayer->setLooping(loop);
}

player_type FFPlayer::playerType() {
    ALOGV("playerType");
    return FF_PLAYER;
}

status_t FFPlayer::invoke(const Parcel &request, Parcel *reply) {
    ALOGV("invoke()");
    return  mPlayer->invoke(request, reply);
}

void FFPlayer::setAudioSink(const sp<AudioSink> &audioSink) {
    MediaPlayerInterface::setAudioSink(audioSink);
    mPlayer->setAudioSink(audioSink);
}

status_t FFPlayer::setParameter(int key, const Parcel &request) {
    ALOGV("setParameter(key=%d)", key);

    return mPlayer->setParameter(key, request);
}

status_t FFPlayer::getParameter(int key, Parcel *reply) {
    ALOGV("getParameter");
    //return INVALID_OPERATION;
    return mPlayer->getParameter(key, reply);
}

status_t FFPlayer::getMetadata(
        const media::Metadata::Filter& ids, Parcel *records) {
        ALOGV("getMetadata");
    using media::Metadata;

    uint32_t flags = mPlayer->flags();

    Metadata metadata(records);

    metadata.appendBool(
            Metadata::kPauseAvailable,
            flags & MediaExtractor::CAN_PAUSE);

    metadata.appendBool(
            Metadata::kSeekBackwardAvailable,
            flags & MediaExtractor::CAN_SEEK_BACKWARD);

    metadata.appendBool(
            Metadata::kSeekForwardAvailable,
            flags & MediaExtractor::CAN_SEEK_FORWARD);

    metadata.appendBool(
            Metadata::kSeekAvailable,
            flags & MediaExtractor::CAN_SEEK);

    return OK;
}

status_t FFPlayer::dump(int fd, const Vector<String16> &args) const {
    return INVALID_OPERATION;
   // return mPlayer->dump(fd, args);
}

}  // namespace android
