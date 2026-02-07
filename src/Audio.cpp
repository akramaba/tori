#include "../inc/Audio.hpp"
#include "../inc/Math.hpp"

#define MINIMP3_FLOAT_OUTPUT
#define MINIMP3_IMPLEMENTATION
#include "../lib/minimp3.h"

#define SOKOL_AUDIO_IMPL
#include "../lib/sokol_audio.h"

#include <algorithm>
#include <cstring>

namespace tori {
    Audio::Sound Audio::sounds_[Audio::MAX_SOUNDS];

    void Audio::stream_callback(float* buffer, int num_frames, int num_channels) {
        std::memset(buffer, 0, num_frames * num_channels * sizeof(float));

        for (int s = 0; s < MAX_SOUNDS; ++s) {
            Sound& snd = sounds_[s];

            if (!snd.playing || snd.pcm_data.empty()) {
                continue;
            }

            double step = (double)snd.src_rate / SAMPLE_RATE;
            size_t total_samples = snd.pcm_data.size() / snd.channels;

            for (int i = 0; i < num_frames; ++i) {
                int frame_idx = (int)snd.pos;

                // End of Stream
                if (frame_idx >= (int)total_samples) {
                    if (snd.loop) {
                        snd.pos = 0;
                        frame_idx = 0;
                    } else {
                        snd.playing = false;
                        break;
                    }
                }

                // Mix Channels
                for (int c = 0; c < num_channels; ++c) {
                    // Map mono sound to stereo output, or stereo to stereo
                    int src_c = (snd.channels == 1) ? 0 : c;
                    
                    // Could be OOB with some MP3s tested
                    size_t sample_idx = frame_idx * snd.channels + src_c;
                    if (sample_idx < snd.pcm_data.size()) {
                        buffer[i * num_channels + c] += snd.pcm_data[sample_idx] * snd.vol;
                    }
                }
                
                snd.pos += step;
            }
        }

        // Hard clip
        for (int i = 0; i < num_frames * num_channels; ++i) {
            if (buffer[i] > 1.0f) {
                buffer[i] = 1.0f;
            }

            if (buffer[i] < -1.0f) {
                buffer[i] = -1.0f;
            }
        }
    }

    void Audio::init() {
        for (auto& s : sounds_) {
            s = Sound();
        }

        saudio_desc desc = {};
        desc.sample_rate = SAMPLE_RATE;
        desc.num_channels = 2;
        desc.stream_cb = stream_callback;
        
        saudio_setup(&desc);
    }

    void Audio::quit() {
        saudio_shutdown();
        
        for (auto& s : sounds_) {
            s.pcm_data.clear();
        }
    }

    AudioHandle Audio::load(const char* path) {
        int slot = -1;
        
        // Find free slot
        for (int i = 0; i < MAX_SOUNDS; ++i) {
            if (sounds_[i].pcm_data.empty()) {
                slot = i;
                break;
            }
        }

        if (slot < 0) {
            return { -1 }; // Full
        }

        // Setup/load MP3

        auto file_data = read_file(path);
        if (file_data.empty()) {
            return { -1 };
        }

        mp3dec_t mp3d;
        mp3dec_init(&mp3d);
        mp3dec_frame_info_t info;
        
        // Decode Loop

        std::vector<float> pcm;
        // Trying an optimization so that the PCM buffer doesn't keep resizing.
        // TODO: Can't just guess 5MB. Look into file features to do this better
        pcm.reserve(1024 * 1024 * 5); 

        uint8_t* mp3_ptr = reinterpret_cast<uint8_t*>(file_data.data());
        int bytes_left = (int)file_data.size();
        
        // Temp buffer for single frame
        float frame_buf[MINIMP3_MAX_SAMPLES_PER_FRAME];

        while (bytes_left > 0) {
            int samples = mp3dec_decode_frame(&mp3d, mp3_ptr, bytes_left, frame_buf, &info);
            
            if (samples > 0) {
                // Append this frame's samples to the master buffer
                int total_samples = samples * info.channels;
                pcm.insert(pcm.end(), frame_buf, frame_buf + total_samples);
            }

            if (info.frame_bytes > 0) {
                mp3_ptr += info.frame_bytes;
                bytes_left -= info.frame_bytes;
            } else {
                break; // Sync error
            }
        }

        if (pcm.empty()) {
            return { -1 };
        }

        // Prepare slot
        sounds_[slot].pcm_data = std::move(pcm);
        sounds_[slot].channels = info.channels;
        sounds_[slot].src_rate = info.hz;
        sounds_[slot].pos = 0;
        sounds_[slot].vol = 1.0f;
        sounds_[slot].loop = false;
        sounds_[slot].playing = false;

        return { slot };
    }

    void Audio::free(AudioHandle snd) {
        if (!snd.is_valid() || snd.id >= MAX_SOUNDS) {
            return;
        }

        sounds_[snd.id].playing = false;
        sounds_[snd.id].pcm_data.clear();
        sounds_[snd.id].pcm_data.shrink_to_fit();
    }

    void Audio::play(AudioHandle snd) {
        if (!snd.is_valid() || snd.id >= MAX_SOUNDS) {
            return;
        }

        if (sounds_[snd.id].pcm_data.empty()) {
            return;
        }
        
        sounds_[snd.id].pos = 0;
        sounds_[snd.id].playing = true;
    }

    void Audio::stop(AudioHandle snd) {
        if (!snd.is_valid() || snd.id >= MAX_SOUNDS) {
            return;
        }

        sounds_[snd.id].playing = false;
    }

    void Audio::set_loop(AudioHandle snd, bool loop) {
        if (!snd.is_valid() || snd.id >= MAX_SOUNDS) {
            return;
        }

        sounds_[snd.id].loop = loop;
    }

    void Audio::set_volume(AudioHandle snd, float vol) {
        if (!snd.is_valid() || snd.id >= MAX_SOUNDS) {
            return;
        }
        
        sounds_[snd.id].vol = vol;
    }
}