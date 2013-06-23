#ifndef LESLIE_H
#define LESLIE_H

#include <jack/jack.h>
#include <stdlib.h>
#include <stdio.h>

#define SINTBL_LEN (2<<12)
#define M_2PI   6.28318530717958647693
#define POLYPH_COUNT 128
#define LATENCY 10
#define KERN_LEN    (LATENCY*2+1)
#define MODBUF_ORDER  15
#define MODBUF_LEN  (2<<MODBUF_ORDER)
#define MODBUF_MASK (MODBUF_LEN-1)

#define LESLIE_DEF_AMP 0.7
#define LESLIE_DEF_MOD 0.5
#define LESLIE_DEF_PHASE 0.5
#define LESLIE_DEF_RATE 4.0
#define LESLIE_DEF_ENABLE false

#define LESLIE_RATE_MIN 0.1
#define LESLIE_RATE_MAX 10
#define LESLIE_RATE_RANGE (LESLIE_RATE_MAX-LESLIE_RATE_MIN)
#define LESLIE_AMP_MIN 0
#define LESLIE_AMP_MAX 1.0
#define LESLIE_MOD_MIN 0
#define LESLIE_MOD_MAX 4.0
#define LESLIE_PHASE_MIN 0
#define LESLIE_PHASE_MAX 0.5

class Leslie
{
    public:
        Leslie();
        ~Leslie();

        float sinc(float x);

        int process(int data_len);

        float setRate(float rate) {return this->rate=rate;}
        float setAmp(float amp) {return this->amp=amp;}
        float setMod(float mod) {return this->mod=mod/1000.0*this->samplerate;}
        float setPhase(float phase) {return this->phase=phase;}
        bool setEnable(bool value) {return this->enable=value;}

        float getRate() {return this->rate;}
        float getAmp() {return this->amp;}
        float getMod() {return this->mod*1000.0/this->samplerate;}
        float getPhase() {return this->phase;}
        bool getEnable() {return this->enable;}

		#ifdef ENABLE_GUI
        void (*gui_update_callback)();
		#endif

    protected:
        float samplerate;
        float phasor;

        bool enable;
        float rate;
        float amp;
        float mod;
        float phase;

    private:
        float sintbl[SINTBL_LEN];
        jack_client_t *client;
        jack_port_t *in_port[2];
        jack_port_t *out_port[2];
        float coeff[POLYPH_COUNT][KERN_LEN];
        jack_default_audio_sample_t modbuf[2][MODBUF_LEN];
        static inline int process_cb(jack_nframes_t data_len,void* p){return static_cast<Leslie*>(p)->process(data_len);}

};
extern Leslie* leslie;
#endif // LESLIE_H
