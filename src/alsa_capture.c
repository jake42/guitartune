/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * alsa_capture.c
 * Copyright (C) cchandel 2009 <cchandel@yahoo.com>
 * 
 * alsa_capture.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * alsa_capture.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <alsa/asoundlib.h>
#include "alsa_capture.h"

int init_alsa(unsigned int rate)
{
	int err;
	
    snd_pcm_hw_params_t *hw_params;

    /*if ((err = snd_pcm_open(&audio_capture_handle, "default", SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC)) < 0) {
        fprintf (stderr, "cannot open audio device: %s\n", snd_strerror(err));
        return -1;
    }*/
	// define device for openmoko
    if ((err = snd_pcm_open(&audio_capture_handle, "plughw", SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC)) < 0) {
        fprintf (stderr, "cannot open audio device: %s\n", snd_strerror(err));
        return -1;
    }
	
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                     snd_strerror(err));
        return -1;
    }
                     
    if((err = snd_pcm_hw_params_any(audio_capture_handle, hw_params)) < 0) {
            fprintf(stderr, "cannot initialize hardware parameter structure(%s)\n",
                     snd_strerror(err));
            return -1;
    }

    if((err = snd_pcm_hw_params_set_access(audio_capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
            fprintf(stderr, "cannot set access type(%s)\n",
                     snd_strerror(err));
            return -1;
    }

    if((err = snd_pcm_hw_params_set_format(audio_capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
            fprintf(stderr, "cannot set sample format(%s)\n",
                     snd_strerror(err));
            return -1;
    }

    if((err = snd_pcm_hw_params_set_rate_near(audio_capture_handle, hw_params, &rate, 0)) < 0) {
        fprintf(stderr, "cannot set sample rate(%s)\n",
                 snd_strerror(err));
        return -1;
    }

    if((err = snd_pcm_hw_params_set_channels(audio_capture_handle, hw_params, 1)) < 0) {
        fprintf(stderr, "cannot set channel count(%s)\n",
                 snd_strerror(err));
        return -1;
    }

    if((err = snd_pcm_hw_params(audio_capture_handle, hw_params)) < 0) {
        fprintf(stderr, "cannot set parameters(%s)\n",
                 snd_strerror(err));
        return -1;
    }

    snd_pcm_hw_params_free(hw_params);	
	// set the alsa state
	//system("alsactl -f ./voice-recording.state restore");
	
    return 0;
}

int get_samples(short* sample, int N)
{
    int err;
	    if ((err = snd_pcm_prepare(audio_capture_handle)) < 0) {
        fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
                 snd_strerror(err));
        return -1;
    }
	snd_pcm_start(audio_capture_handle);
    //if ((err = 
	snd_pcm_readi(audio_capture_handle, sample, N);//) < N) {
        //if (err == -EPIPE) return 0;
        //fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror(err));
        //exit(1);
    //}
	snd_pcm_drop(audio_capture_handle);
    return 0;	
}

/*int init_mixer()
{
	snd_hctl_t *hctl;
	
	err = snd_hctl_open(&hctl, "hw:0", 0));
	err = snd_hctl_load(hctl);
	
	snd_ctl_elem_id_t *id;
	
	snd_ctl_elem_id_alloca(&id);
	snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
	snd_ctl_elem_id_set_name(id, "Master");
	
	snd_hctl_elem_t *elem = snd_hctl_find_elem(hctl, id);
	
	snd_ctl_elem_value_t *control;
	
	snd_ctl_elem_value_alloca(&control);
	snd_ctl_elem_value_set_id(control, id);    
	snd_ctl_elem_value_set_integer(control, 0, 66);

	err = snd_hctl_elem_write(elem, control);
}*/
