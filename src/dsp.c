/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

// downsample the data by a factor of 2. Also applies a triangular window for
// antialiasing. Credit: mumart[AT]gmail[DOT]com. From musicdsp.org

#include "dsp.h"
#include "alsa_capture.h"
#include "fftw3.h"

extern float max_amp;
extern int new_freq;

// inline this code
void 
downsample( float *input_buf, float *output_buf, int output_count ) {
	int input_idx, input_end, output_idx; 
	float output_sam;
	input_idx = output_idx = 0;
	input_end = output_count * 2;
	float filter_state=1.0;

	while( input_idx < input_end ) {
		output_sam = filter_state + ( input_buf[ input_idx++ ] *0.5 );
		filter_state = input_buf[ input_idx++ ] *0.25;
		output_buf[ output_idx++ ] = output_sam + filter_state;
	}
}

// get data from /dev/dsp, apply a FFT on it and do HPS.
// return the freq with max amplitude (hopefully ;-)
void 
get_max_amp(void)
{
	float max;
	double temp;
	max = temp = 0.0;
	int i=0,maxi=0;
	double prefft[N];
	fftw_complex postfft[N];
	fftw_plan plan;
	float amp[N], amp_down2[N], amp_down4[N];
	
	while (1) {
	  get_samples(sample, N);
	  // add function to check for volume level
	  // do more calculations if volume is above a threshold else return
	
	  for(i=0;i<N;i++)  {
		prefft[i]=((float)sample[i] * mycosine[i]);
	  }
		
	  // do FFT	
	  plan = fftw_plan_dft_r2c_1d(N, prefft, postfft, 0);
	  fftw_execute(plan);
		
	  // calculate amplitude of the freq's
	  for (i = 0; i < 9000; i++ ) {
		amp[i] = postfft[i][0] * postfft[i][0] + postfft[i][1] * postfft[i][1];
	  }

	  //downsample by 2
	  downsample(amp, amp_down2, 4500);

	  //downsample by 2 more
	  downsample(amp_down2, amp_down4, 2250);
	  
	  // downsample by a factor of three also
	  max_amp = 0;
	  max = 0.0;
		
	  // multiply the amplitudes
	  for (i = 20; i < 1000; i++) {
		  temp = amp[i] * amp_down2[i] * amp_down4[i];
		  if (temp > max) {
		    max = temp;
		    maxi = i;
		  }
	  } 
	max_amp = maxi;
	new_freq = 1;
	}
}
