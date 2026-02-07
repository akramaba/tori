#pragma once

#include <string_view>
#include <vector>

namespace tori {
    struct AudioHandle {
        int id = -1;

        bool is_valid() const { 
            return id >= 0;
        }
    };

    class Audio {
    public:
        static void init();
        static void quit();
        static AudioHandle load(const char* path);
        
        // Controls
        static void play(AudioHandle snd);
        static void stop(AudioHandle snd);
        static void set_loop(AudioHandle snd, bool loop);
        static void set_volume(AudioHandle snd, float vol);
        
        // Resources
        static void free(AudioHandle snd);
    private:
        static constexpr int MAX_SOUNDS = 32;
        static constexpr int SAMPLE_RATE = 44100;

        struct Sound {
            std::vector<float> pcm_data;
            int channels = 0;
            int src_rate = 0;
            double pos = 0.0;
            float vol = 1.0f;
            bool loop = false;
            bool playing = false;
        };

        static Sound sounds_[MAX_SOUNDS];
        
        // Callback for sokol_audio
        static void stream_callback(float* buffer, int num_frames, int num_channels);
    };
}