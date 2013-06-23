#include "leslie.h"
#include "math.h"
#include <jack/jack.h>
#include <jack/midiport.h>
Leslie* leslie=new Leslie();
using namespace std;
Leslie::Leslie()
{
	#ifdef ENABLE_GUI
    this->gui_update_callback=NULL;
	#endif

    //generate sin table
    for(int i=0;i<SINTBL_LEN;i++){
        sintbl[i]=(sin(float(i)/SINTBL_LEN*M_2PI)+1)/2;
    }

    //generate sinc interpolation tables
    float offset=0.5;
    for(int i=0;i<POLYPH_COUNT;i++){
        for (int x=0;x<(2*LATENCY);x++){
            offset=float(i)/(POLYPH_COUNT-1);
            this->coeff[i][x]=
                sinc(x-LATENCY-offset+1)*
                sinc(2*(x+1-offset)/float(KERN_LEN)-1 );
        }
    }

    //Jack init
    if ((client = jack_client_open ("leslie",JackNoStartServer,NULL)) == 0) {
            fprintf (stderr, "jack server not running?\n");
            throw;
    }
    this->phasor=0;
    this->samplerate=jack_get_sample_rate(client);
    this->in_port [0]  =jack_port_register(client,"in.left"  ,JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput ,0);
    this->in_port [1]  =jack_port_register(client,"in.right" ,JACK_DEFAULT_AUDIO_TYPE,JackPortIsInput ,0);
    this->out_port[0]  =jack_port_register(client,"out.left" ,JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
    this->out_port[1]  =jack_port_register(client,"out.right",JACK_DEFAULT_AUDIO_TYPE,JackPortIsOutput,0);
    jack_set_process_callback(client, Leslie::process_cb, this);

    this->setAmp(LESLIE_DEF_AMP);
    this->setMod(LESLIE_DEF_MOD);
    this->setPhase(LESLIE_DEF_PHASE);
    this->setRate(LESLIE_DEF_RATE);
    this->setEnable(LESLIE_DEF_ENABLE);

    if (jack_activate(client)) {
        fprintf (stderr, "cannot activate client");
        throw;
    }
}

int Leslie::process(int data_len)
{
    jack_default_audio_sample_t *indata [2];
    jack_default_audio_sample_t *outdata[2];
    float val;
    static int mod_idx=0;
    float f_mod;
    int i_mod;
    float f_coeff;
    int i_coeff;

    static float rate_filt=0;
    static float amp_filt=0;
    static float mod_filt=0;
    static float phase_filt=0;
    static float mix=0;

    indata[0]= (jack_default_audio_sample_t *)jack_port_get_buffer(this->in_port [0],data_len);
    indata[1]= (jack_default_audio_sample_t *)jack_port_get_buffer(this->in_port [1],data_len);
    outdata[0]=(jack_default_audio_sample_t *)jack_port_get_buffer(this->out_port[0],data_len);
    outdata[1]=(jack_default_audio_sample_t *)jack_port_get_buffer(this->out_port[1],data_len);

    for(int i=0;i<data_len;i++)
    {
        //filter the input signals
        rate_filt=0.999*rate_filt+0.001*this->rate;
        amp_filt=0.999*amp_filt+0.001*this->amp;
        phase_filt=0.999*phase_filt+0.001*this->phase;
        mod_filt=0.999*mod_filt+0.001*this->mod;

        //TODO: try switch at zero crossing instead of crossfading
        if(this->enable){
            mix+=0.0002;
            if (mix>1) mix=1;
        }else if (mix>0) mix-=0.0002;

        if(mix>0){
            //Left channel
            //-----------------------------------
            this->modbuf[0][mod_idx]=indata[0][i];


            //sin lookup (linear interpolation)
            val=this->phasor*SINTBL_LEN;
            val=sintbl[int(val)]+(sintbl[(int(val)+1)%SINTBL_LEN]-sintbl[int(val)])*(val-int(val));

            //polyphase interpolation for modulation
            //linear interpolation between tables of the polyphase filter
            f_mod=(1-val)*mod_filt;
            i_mod=int(f_mod);
            f_mod-=i_mod;
            f_coeff=f_mod*(POLYPH_COUNT-1);
            i_coeff=int(f_coeff);
            f_coeff-=i_coeff;
            outdata[0][i]=0;
            for(int x=0;x<KERN_LEN;x++){
                outdata[0][i]+=this->modbuf[0][(mod_idx-i_mod-x)&MODBUF_MASK]*(
                    coeff[i_coeff][x]*(1-f_coeff)+coeff[(i_coeff+1)%(POLYPH_COUNT+1)][x]*(f_coeff)
                );
            }
            outdata[0][i]*=(val*amp_filt+1-amp_filt)*mix;
            outdata[0][i]+=indata[0][i]*(1-mix);

            //Right channel
            //-----------------------------------
            this->modbuf[1][mod_idx]=indata[1][i];


            //sin lookup (linear interpolation)
            val=this->phasor+phase_filt;
            val=(val-int(val))*SINTBL_LEN;
            val=sintbl[int(val)]+(sintbl[(int(val)+1)%SINTBL_LEN]-sintbl[int(val)])*(val-int(val));

            //polyphase interpolation for modulation
            //linear interpolation between tables of the polyphase filter
            f_mod=(1-val)*mod_filt;
            i_mod=int(f_mod);
            f_mod-=i_mod;
            f_coeff=f_mod*(POLYPH_COUNT-1);
            i_coeff=int(f_coeff);
            f_coeff-=i_coeff;
            outdata[1][i]=0;
            for(int x=0;x<KERN_LEN;x++){
                outdata[1][i]+=this->modbuf[1][(mod_idx-i_mod-x)&MODBUF_MASK]*(
                    coeff[i_coeff][x]*(1-f_coeff)+coeff[(i_coeff+1)%(POLYPH_COUNT+1)][x]*(f_coeff)
                );
            }
            outdata[1][i]*=(val*amp_filt+1-amp_filt)*mix;
            outdata[1][i]+=indata[1][i]*(1-mix);
        }else{
            outdata[0][i]=indata[0][i];
            outdata[1][i]=indata[1][i];
        }


        //Advance indexes/phasors
        //-----------------------------------
        mod_idx=(mod_idx+1)&MODBUF_MASK;
        this->phasor+=rate_filt/this->samplerate;
        this->phasor-=int(this->phasor);

    }
    return 0;
}

Leslie::~Leslie()
{
}

float Leslie::sinc(float x)
{
    if (x==0) return 1;
    return sin(M_PI*x)/(M_PI*x);
}

