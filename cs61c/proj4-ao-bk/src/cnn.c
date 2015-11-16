#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "timestamp.c"

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

// Include OpenMP
#include <omp.h>

// Vol ------------------------------------------------------------------------

// Volumes are used to represent the activations (i.e., state) between the
// different layers of the CNN. They all have three dimensions. The inter-
// pretation of their content depends on the layer that produced them. Before
// the first iteration, the Volume holds the data of the image we want to
// classify (the depth are the three color dimensions). After the last stage
// of the CNN, the Volume holds the probabilities that an image is part of
// a specific category.

/*
 * Represents a three-dimensional array of numbers, and its size. The numbers
 * at (x,y,d) are stored in array w at location ((v->sx * y)+x)*v->depth+d.
 */

typedef struct vol {
  uint64_t sx,sy,depth;
  double* w;
} vol_t;

/*
 * Set the value at a specific entry of the array.
 */

static inline double get_vol(vol_t* v, int x, int y, int d) {
  return v->w[((v->sx * y)+x)*v->depth+d];
}

/*
 * Get the value at a specific entry of the array.
 */

static inline void set_vol(vol_t* v, int x, int y, int d, double val) {
  v->w[((v->sx * y)+x)*v->depth+d] = val;
}

/*
 * Allocate a new array with specific dimensions and default value v.
 */

static vol_t* make_vol(int sx, int sy, int d, double v) {
  vol_t* out = (vol_t*)malloc(sizeof(struct vol));
  out->w = (double*)malloc(sizeof(double)*(sx*sy*d));
  out->sx = sx;
  out->sy = sy;
  out->depth = d;

  for (int x = 0; x < sx; x++)
    for (int y = 0; y < sy; y++)
      for (int z = 0; z < d; z++)
        set_vol(out, x, y, z, v);

  return out;
}

/*
 * Copy the contents of one Volume to another (assuming same dimensions).
 */

static vol_t* copy_vol(vol_t* dest, vol_t* src) {
  for (int x = 0; x < dest->sx; x++)
    for (int y = 0; y < dest->sy; y++)
      for (int z = 0; z < dest->depth; z++)
        set_vol(dest, x, y, z, get_vol(src, x, y, z));
}

/*
 * Deallocate the array.
 */
void free_vol(vol_t* v) {
  free(v->w);
  free(v);
}

// A note about layers --------------------------------------------------------

/*
 * What follows are the different layers of the CNN. You will not have to
 * understand what these layers are actually doing. In general terms, each
 * layer performs a "forward" operation on a batch of inputs. During this
 * forward operation, the layer takes a set of input Volumes and transforms
 * them into a set of output Volumes (one output for each input). What differs
 * is the operation performed by each layer.
 *
 * In addition to the _forward function, each layer also provides a data
 * structure, holding (fixed) parameters for that layer, a make_ function to
 * allocate an instance of the layer with a particular set of parameters and
 * a load function to load training data for that layer from a file. Note that
 * you will not have to make any changes to any of these functions. The only
 * function you need to consider is the _forward function.
 */

// Convolutional Layer --------------------------------------------------------

typedef struct conv_layer {
  // required
  int out_depth;
  int sx;
  int in_depth;
  int in_sx;
  int in_sy;

  // optional
  int sy;
  int stride;
  int pad;
  double l1_decay_mul;
  double l2_decay_mul;

  // computed
  int out_sx;
  int out_sy;
  double bias;
  vol_t* biases;
  vol_t** filters;
} conv_layer_t;

conv_layer_t* make_conv_layer(int in_sx, int in_sy, int in_depth,
                              int sx, int filters, int stride, int pad) {
  conv_layer_t* l = (conv_layer_t*)malloc(sizeof(conv_layer_t));

  // required
  l->out_depth = filters;
  l->sx = sx;
  l->in_depth = in_depth;
  l->in_sx = in_sx;
  l->in_sy = in_sy;
    
  // optional
  l->sy = l->sx;
  l->stride = stride;
  l->pad = pad;
  l->l1_decay_mul = 0.0;
  l->l2_decay_mul = 1.0;

  // computed
  l->out_sx = floor((l->in_sx + l->pad * 2 - l->sx) / l->stride + 1);
  l->out_sy = floor((l->in_sy + l->pad * 2 - l->sy) / l->stride + 1);

  l->filters = (vol_t**)malloc(sizeof(vol_t*)*filters);
  for (int i = 0; i < filters; i++) {
    l->filters[i] = make_vol(l->sx, l->sy, l->in_depth, 0.0);
  }

  l->bias = 0.0;
  l->biases = make_vol(1, 1, l->out_depth, l->bias);

  return l;
}


void conv_forward3(conv_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
  //printf("start: %d \n",start);
  //printf("end: %d \n", end);
  
    vol_t* V = in[0];
    
    //printf("V_depth: %d \n", V_depth);
    vol_t* A = out[0];
    
    int ay;
    int ax;
    int fx;
    int fy;
    int oy;
    int ox;
    int d;
    int f_depth;
    int x;
    int y;
    double a;
    int base1;
    int base2;
    __m256d temp1;
    __m256d temp2;
   
    
    //printf("l_out_depth: %d \n", l_out_depth);

    for(d = 0; d < 20; d++) {
      vol_t* f = l->filters[d]; // f DEFINED
      
      f_depth = f->depth;
   // printf( "f_depth: %d \n", f_depth);
     
      y = -2;
      for(ay = 0; ay < 8; y += 1, ay++) 
      {
         x = -2;
        for( ax=0; ax < 8; x += 1, ax++) 
        {
           a = 0.0;
           __m256d vect = _mm256_setzero_pd();
          for( fy = 0; fy < 5; fy++) 
          {
             oy = y + fy;
            for( fx = 0; fx < 5; fx++) 
            {
               ox = x + fx;
              if(oy >= 0 && oy < 8 && ox >=0 && ox < 8) 
              {
                  base1 = ((5 * fy)+fx)*f_depth;
                  base2 = ((8 * oy)+ox)*20;
                  if (f_depth ==3){
                    for(int i = 0; i< 3; i++){
                      a+= f->w[base1+i]*V->w[base2+i];

                    }
                  }
                  else if (f_depth == 16){
                  
                
                  
                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +4));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 + 4));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +8));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 +8));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+12));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+12));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  

                }

                  
                  else if(f_depth == 20){
                  
                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));

                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +4));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 + 4));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +8));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 +8));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+12));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+12));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                   temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+16));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+16));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                 
                  }

                  
              }
          }
        }

         double p[4] = {0.0, 0.0, 0.0, 0.0};
                  _mm256_store_pd(p, vect);
                  a += p[0] + p[1] + p[2] + p[3];
          a += l->biases->w[d];
          
          set_vol(A, ax, ay, d, a);
        }
      }
    }
  
}

void conv_forward1(conv_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
  //printf("start: %d \n",start);
  //printf("end: %d \n", end);
  
    vol_t* V = in[0];
    
    //printf("V_depth: %d \n", V_depth);
    vol_t* A = out[0];
    
    int ay;
    int ax;
    int fx;
    int fy;
    int oy;
    int ox;
    int d;
    int f_depth;
    int x;
    int y;
    double a;
    int base1;
    int base2;
   __m256d temp1;
   __m256d temp2; 
   __m256d vect;
   
    //printf("l_out_depth: %d \n", l_out_depth);

    for( d = 0; d < 16; d++) {
      vol_t* f = l->filters[d]; // f DEFINED
      
      f_depth = f->depth;
   // printf( "f_depth: %d \n", f_depth);
     
      y = -2;
      for(ay = 0; ay < 32; y += 1, ay++) 
      {
         x = -2;
        for( ax=0; ax < 32; x += 1, ax++) 
        {
           a = 0.0;
            vect = _mm256_setzero_pd();
          for( fy = 0; fy < 5; fy++) 
          {
             oy = y + fy;
            for( fx = 0; fx < 5; fx++) 
            {
               ox = x + fx;

              if(oy >= 0 && oy < 32 && ox >=0 && ox < 32) 
              {
                  base1 = ((5 * fy)+fx)*f_depth;
                  base2 = ((32 * oy)+ox)*3;
                  if (f_depth ==3){
                    for(int i = 0; i< 3; i++){
                      a+= f->w[base1+i]*V->w[base2+i];

                    }
                  }
                  else if (f_depth == 16){
                 
                  
                  
                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +4));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 + 4));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +8));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 +8));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+12));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+12));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  
                 

                }

                  
                  else if(f_depth == 20){
                   __m256d vect = _mm256_setzero_pd();
                
                 
                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));

                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +4));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 + 4));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +8));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 +8));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+12));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+12));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                   temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+16));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+16));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  }

                  
              }
          }
        }

                  double p[4] = {0.0, 0.0, 0.0, 0.0};
                  _mm256_store_pd(p, vect);
                  a += p[0] + p[1] + p[2] + p[3];
          a += l->biases->w[d];
         
          set_vol(A, ax, ay, d, a);
        }
      }
    }
  
}

void conv_forward2(conv_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
  //printf("start: %d \n",start);
  //printf("end: %d \n", end);
  
    vol_t* V = in[0];
    
    //printf("V_depth: %d \n", V_depth);
    vol_t* A = out[0];
    
    int ay;
    int ax;
    int fx;
    int fy;
    int oy;
    int ox;
    int d;
    int f_depth;
    int x;
    int y;
    double a;
    int base1;
    int base2;
    __m256d temp1;
    __m256d temp2;
    __m256d vect;
    
    //printf("l_out_depth: %d \n", l_out_depth);

    for(int d = 0; d < 20; d++) {
      vol_t* f = l->filters[d]; // f DEFINED
      
      f_depth = f->depth;
   // printf( "f_depth: %d \n", f_depth);
     
      y = -2;
      for(ay = 0; ay < 16; y += 1, ay++) 
      {
         x = -2;
        for( ax=0; ax < 16; x += 1, ax++) 
        {
           a = 0.0;
           vect = _mm256_setzero_pd();
          for( fy = 0; fy < 5; fy++) 
          {
             oy = y + fy;
            for( fx = 0; fx < 5; fx++) 
            {
               ox = x + fx;
               

              if(oy >= 0 && oy < 16 && ox >=0 && ox < 16) 
              {
                  base1 = ((5 * fy)+fx)*f_depth;
                  base2 = ((16 * oy)+ox)*16;
                  if (f_depth ==3){
                    for(int i = 0; i< 3; i++){
                      a+= f->w[base1+i]*V->w[base2+i];

                    }
                  }
                  else if (f_depth == 16){
                  
                  
                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +4));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 + 4));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +8));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 +8));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+12));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+12));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  
                  
                  

                }
                  else if(f_depth == 20){

                  __m256d temp1;
                  __m256d temp2; 
                 
                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));

                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +4));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 + 4));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1 +8));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2 +8));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));



                  temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+12));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+12));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                   temp1 = _mm256_loadu_pd((__m256d*)((f->w)+base1+16));
                  temp2 = _mm256_loadu_pd((__m256d*)((V->w)+base2+16));
                  vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));


                
                  }

                  
              }


                  


          }
        }
        double p[4] = {0.0, 0.0, 0.0, 0.0};
                  _mm256_store_pd(p, vect);
                  a += p[0] + p[1] + p[2] + p[3];
          a += l->biases->w[d];
          
          set_vol(A, ax, ay, d, a);
        }
      }
    }
  
}




void conv_load(conv_layer_t* l, const char* fn) {
  int sx, sy, depth, filters;

  FILE* fin = fopen(fn, "r");

  fscanf(fin, "%d %d %d %d", &sx, &sy, &depth, &filters);
  assert(sx == l->sx);
  assert(sy == l->sy);
  assert(depth == l->in_depth);
  assert(filters == l->out_depth);
  int d;
  int x;
  int y;
  int z;
  
  for(d = 0; d < l->out_depth; d++)
    for ( x = 0; x < sx; x++)
      for ( y = 0; y < sy; y++)
        for ( z = 0; z < depth; z++) {
          double val;
          fscanf(fin, "%lf", &val);
          set_vol(l->filters[d], x, y, z, val);
        }

  for(d = 0; d < l->out_depth; d++) {
    double val;
    fscanf(fin, "%lf", &val);
    set_vol(l->biases, 0, 0, d, val);
  }

  fclose(fin);
}

// Relu Layer -----------------------------------------------------------------

typedef struct relu_layer {
  // required
  int in_depth;
  int in_sx;
  int in_sy;

  // computed
  int out_depth;
  int out_sx;
  int out_sy;
} relu_layer_t;

relu_layer_t* make_relu_layer(int in_sx, int in_sy, int in_depth) {
  relu_layer_t* l = (relu_layer_t*)malloc(sizeof(relu_layer_t));

  // required
  l->in_depth = in_depth;
  l->in_sx = in_sx;
  l->in_sy = in_sy;

  // computed
  l->out_sx = l->in_sx;
  l->out_sy = l->in_sy;
  l->out_depth = l->in_depth;

  return l;
}

void relu_forward(relu_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
    int mul =  l->in_sx*l->in_sy*l->in_depth;

    for (int i = 0; i < mul; i++) {



      out[0]->w[i] = (in[0]->w[i] < 0.0) ? 0.0 : in[0]->w[i];
    }
  
}

// Pool Layer -----------------------------------------------------------------

typedef struct pool_layer {
  // required
  int sx;
  int in_depth;
  int in_sx;
  int in_sy;

  // optional
  int sy;
  int stride;
  int pad;

  // computed
  int out_depth;
  int out_sx;
  int out_sy;
} pool_layer_t;

pool_layer_t* make_pool_layer(int in_sx, int in_sy, int in_depth,
                              int sx, int stride) {
  pool_layer_t* l = (pool_layer_t*)malloc(sizeof(pool_layer_t));

  // required
  l->sx = sx;
  l->in_depth = in_depth;
  l->in_sx = in_sx;
  l->in_sy = in_sy;

  // optional
  l->sy = l->sx;
  l->stride = stride;
  l->pad = 0;

  // computed
  l->out_depth = in_depth;
  l->out_sx = floor((l->in_sx + l->pad * 2 - l->sx) / l->stride + 1);
  l->out_sy = floor((l->in_sy + l->pad * 2 - l->sy) / l->stride + 1);

  return l;
}

void pool_forward(pool_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
 
    vol_t* V = in[0];
    vol_t* A = out[0];
    int x;
    int y;
    
    int d;
    int fx;
    int fy;
    int ox;
    int oy;
    int ax;
    int ay;
    double v;
    double a;

    //printf("%d \n", l->out_sx);
    for(d=0;d<l->out_depth;d++) {
      x = 0;
      
      for( ax=0; ax<l->out_sx; x+=2,ax++) {
        y = 0;
        
        for( ay=0; ay<l->out_sy; y+=2,ay++) {
  
          a = -99999;
          for(fx=0;fx<2;fx++) {
           // printf("%d \n", l-> sx);
            for( fy=0;fy<2;fy++) {
               oy = y+fy;
               ox = x+fx;
              if(oy>=0 && oy<V->sy && ox>=0 && ox<V->sx) {
                v = get_vol(V, ox, oy, d);
                if(v > a) { a = v; }
              }
            }
          }
          
          set_vol(A, ax, ay, d, a);
        }
      }
    
  }
}

// FC Layer -------------------------------------------------------------------

typedef struct fc_layer {
  // required
  int out_depth;
  int in_depth;
  int in_sx;
  int in_sy;

  // optional
  double l1_decay_mul;
  double l2_decay_mul;

  // computed
  int out_sx;
  int out_sy;
  int num_inputs;
  double bias;
  vol_t* biases;
  vol_t** filters;
} fc_layer_t;

fc_layer_t* make_fc_layer(int in_sx, int in_sy, int in_depth,
                          int num_neurons) {
  fc_layer_t* l = (fc_layer_t*)malloc(sizeof(fc_layer_t));

  // required
  l->out_depth = num_neurons;
  l->in_depth = in_depth;
  l->in_sx = in_sx;
  l->in_sy = in_sy;
    
  // optional
  l->l1_decay_mul = 0.0;
  l->l2_decay_mul = 1.0;

  // computed
  l->num_inputs = l->in_sx * l->in_sy * l->in_depth;
  l->out_sx = 1;
  l->out_sy = 1;

  l->filters = (vol_t**)malloc(sizeof(vol_t*)*num_neurons);
  for (int i = 0; i < l->out_depth; i++) {
    l->filters[i] = make_vol(1, 1, l->num_inputs, 0.0);
  }

  l->bias = 0.0;
  l->biases = make_vol(1, 1, l->out_depth, l->bias);

  return l;
}

void fc_forward(fc_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
  
    vol_t* V = in[0];
    vol_t* A = out[0];
    int i;
    int d;
    double a;
    double* array1;
    double* array2;
    __m256d temp1;
    __m256d temp2;
    __m256d vect;
    for(i=0;i<10;i++) {
      a = 0.0;
      vect = _mm256_setzero_pd();



      for(d=0;d<320;d+=4) {
            temp1 = _mm256_loadu_pd((__m256d*)((V->w)+d));
            temp2 = _mm256_loadu_pd((__m256d*)((l->filters[i]->w)+d));
            vect = _mm256_add_pd(vect, _mm256_mul_pd(temp1, temp2));
        // a += V->w[d] * l->filters[i]->w[d];
      }

        double p[4] = {0.0, 0.0, 0.0, 0.0};
        _mm256_store_pd(p, vect);
        a += p[0] + p[1] + p[2] + p[3];
      a += l->biases->w[i];
      A->w[i] = a;
    }
  
}

void fc_load(fc_layer_t* l, const char* fn) {
  FILE* fin = fopen(fn, "r");

  int num_inputs;
  int out_depth;
  fscanf(fin, "%d %d", &num_inputs, &out_depth);
  assert(out_depth == l->out_depth);
  assert(num_inputs == l->num_inputs);

  for(int i = 0; i < l->out_depth; i++)
    for(int d = 0; d < l->num_inputs; d++) {
      double val;
      fscanf(fin, "%lf", &val);
      l->filters[i]->w[d] = val;
    }

  for(int i = 0; i < l->out_depth; i++) {
    double val;
    fscanf(fin, "%lf", &val);
    l->biases->w[i] = val;
  }

  fclose(fin);
}

// Softmax Layer --------------------------------------------------------------

// Maximum supported out_depth
#define MAX_ES 16

typedef struct softmax_layer {
  // required
  int in_depth;
  int in_sx;
  int in_sy;
  double* es; 

  // computed
  int out_depth;
  int out_sx;
  int out_sy;
} softmax_layer_t;

softmax_layer_t* make_softmax_layer(int in_sx, int in_sy, int in_depth) {
  softmax_layer_t* l = (softmax_layer_t*)malloc(sizeof(softmax_layer_t));

  // required
  l->in_depth = in_depth;
  l->in_sx = in_sx;
  l->in_sy = in_sy;

  // computed
  l->out_sx = 1;
  l->out_sy = 1;
  l->out_depth = l->in_sx * l->in_sy * l->in_depth;

  l->es = (double*)malloc(sizeof(double)*l->out_depth);

  return l;
}

void softmax_forward(softmax_layer_t* l, vol_t** in, vol_t** out, int start, int end) {
  double es[MAX_ES];

 
    vol_t* V = in[0];
    vol_t* A = out[0];
  
    // compute max activation
    double amax = V->w[0];
    for(int i=1;i<l->out_depth;i++) {
      if(V->w[i] > amax) amax = V->w[i];
    }
  
    // compute exponentials (carefully to not blow up)
    double esum = 0.0;
    for(int i=0;i<l->out_depth;i++) {
      double e = exp(V->w[i] - amax);
      esum += e;
      es[i] = e;
    }
  
    // normalize and output to sum to one
    for(int i=0;i<l->out_depth;i++) {
      es[i] /= esum;
      A->w[i] = es[i];
    }
  
}

// Neural Network -------------------------------------------------------------

/*
 * This represents the CNN we will use in this project. It consists of 11
 * layers, which means that there are 12 volumes of data (where the first one
 * is the input data and the last one the classification result).
 */

#define LAYERS 11

typedef struct network {
  vol_t* v[12];
  conv_layer_t* l0;
  relu_layer_t* l1;
  pool_layer_t* l2;
  conv_layer_t* l3;
  relu_layer_t* l4;
  pool_layer_t* l5;
  conv_layer_t* l6;
  relu_layer_t* l7;
  pool_layer_t* l8;
  fc_layer_t* l9;
  softmax_layer_t* l10;
} network_t;

/*
 * Instantiate our specific CNN.
 */

network_t* make_network() {
  network_t* net = (network_t*)malloc(sizeof(network_t));
  net->v[0] = make_vol(32, 32, 3, 0.0);
  net->l0 = make_conv_layer(32, 32, 3, 5, 16, 1, 2);
  net->v[1] = make_vol(net->l0->out_sx, net->l0->out_sy, net->l0->out_depth, 0.0);
  net->l1 = make_relu_layer(net->v[1]->sx, net->v[1]->sy, net->v[1]->depth);
  net->v[2] = make_vol(net->l1->out_sx, net->l1->out_sy, net->l1->out_depth, 0.0);
  net->l2 = make_pool_layer(net->v[2]->sx, net->v[2]->sy, net->v[2]->depth, 2, 2);
  net->v[3] = make_vol(net->l2->out_sx, net->l2->out_sy, net->l2->out_depth, 0.0);
  net->l3 = make_conv_layer(net->v[3]->sx, net->v[3]->sy, net->v[3]->depth, 5, 20, 1, 2);
  net->v[4] = make_vol(net->l3->out_sx, net->l3->out_sy, net->l3->out_depth, 0.0);
  net->l4 = make_relu_layer(net->v[4]->sx, net->v[4]->sy, net->v[4]->depth);
  net->v[5] = make_vol(net->l4->out_sx, net->l4->out_sy, net->l4->out_depth, 0.0);
  net->l5 = make_pool_layer(net->v[5]->sx, net->v[5]->sy, net->v[5]->depth, 2, 2);
  net->v[6] = make_vol(net->l5->out_sx, net->l5->out_sy, net->l5->out_depth, 0.0);
  net->l6 = make_conv_layer(net->v[6]->sx, net->v[6]->sy, net->v[6]->depth, 5, 20, 1, 2);
  net->v[7] = make_vol(net->l6->out_sx, net->l6->out_sy, net->l6->out_depth, 0.0);
  net->l7 = make_relu_layer(net->v[7]->sx, net->v[7]->sy, net->v[7]->depth);
  net->v[8] = make_vol(net->l7->out_sx, net->l7->out_sy, net->l7->out_depth, 0.0);
  net->l8 = make_pool_layer(net->v[8]->sx, net->v[8]->sy, net->v[8]->depth, 2, 2);
  net->v[9] = make_vol(net->l8->out_sx, net->l8->out_sy, net->l8->out_depth, 0.0);
  net->l9 = make_fc_layer(net->v[9]->sx, net->v[9]->sy, net->v[9]->depth, 10);
  net->v[10] = make_vol(net->l9->out_sx, net->l9->out_sy, net->l9->out_depth, 0.0);
  net->l10 = make_softmax_layer(net->v[10]->sx, net->v[10]->sy, net->v[10]->depth);
  net->v[11] = make_vol(net->l10->out_sx, net->l10->out_sy, net->l10->out_depth, 0.0);
  return net;
}

/* 
 * Free our specific CNN.
 */

void free_network(network_t* net) {
  for (int i = 0; i < 12; i++)
    free_vol(net->v[i]);

  free(net->l0);
  free(net->l1);
  free(net->l2);
  free(net->l3);
  free(net->l4);
  free(net->l5);
  free(net->l6);
  free(net->l7);
  free(net->l8);
  free(net->l9);
  free(net->l10);

  free(net);
}

/*
 * We organize data as "batches" of volumes. Each batch consists of a number of samples,
 * each of which contains a volume for every intermediate layer. Say we have L layers
 * and a set of N input images. Then batch[l][n] contains the volume at layer l for
 * input image n.
 *
 * By using batches, we can process multiple images at once in each run of the forward
 * functions of the different layers.
 */

typedef vol_t** batch_t;

/*
 * This function allocates a new batch for the network old_net with size images.
 */

batch_t* make_batch(network_t* old_net, int size) {
  
  batch_t* out = (batch_t*)malloc(sizeof(vol_t**)*(12));
  for (int i = 0; i < 12; i++) {
    out[i] = (vol_t**)malloc(sizeof(vol_t*));
    
      out[i][0] = make_vol(old_net->v[i]->sx, old_net->v[i]->sy, old_net->v[i]->depth, 0.0);
    
  }

  return out;
}

/*
 * Free a previously allocated batch.
 */

void free_batch(batch_t* v, int size) {
  
  for (int i = 0; i < 12; i++) {
  
      free_vol(v[i][0]);
    
    free(v[i]);
  }
  free(v);
}

/*
 * Apply our network to a specific batch of inputs. The batch has to be given
 * as input to v and start/end are the first and the last image in that batch
 * to process (start and end are inclusive).
 */

void net_forward(network_t* net, batch_t* v, int start, int end) {
 

 // uint64_t x = timestamp_us();
 // printf("%f start \n", (double)x );

 //  conv_forward(net->l0, v[0], v[1], start, end);
 //  x = timestamp_us();
 // printf("%f conv_forward \n", (double)x );

 //  relu_forward(net->l1, v[1], v[2], start, end);

 // x = timestamp_us();
 // printf("%f relu_forward \n", (double)x );

 //  pool_forward(net->l2, v[2], v[3], start, end);

 // x = timestamp_us();
 // printf("%f pool_forward \n", (double)x );

 //  conv_forward(net->l3, v[3], v[4], start, end);

 // x = timestamp_us();
 // printf("%f conv_forward \n", (double)x );

 //  relu_forward(net->l4, v[4], v[5], start, end);

 // x = timestamp_us();
 // printf("%f relu_forward \n", (double)x );

 //  pool_forward(net->l5, v[5], v[6], start, end);

 // x = timestamp_us();
 // printf("%f pool_forward \n", (double)x );

 //  conv_forward(net->l6, v[6], v[7], start, end);

 // x = timestamp_us();
 // printf("%f conv_forward \n", (double)x );

 //  relu_forward(net->l7, v[7], v[8], start, end);

 // x = timestamp_us();
 // printf("%f relu_forward \n", (double)x );

 //  pool_forward(net->l8, v[8], v[9], start, end);

 // x = timestamp_us();
 // printf("%f pool_forward \n", (double)x );

 //  fc_forward(net->l9, v[9], v[10], start, end);

 // x = timestamp_us();
 // printf("%f fc_forward \n", (double)x );

 //  softmax_forward(net->l10, v[10], v[11], start, end);

 // x = timestamp_us();
 // printf("%f softmax \n", (double)x );


  //printf("conv 1 \n");
  conv_forward1(net->l0, v[0], v[1], start, end);
  
  relu_forward(net->l1, v[1], v[2], start, end);



  pool_forward(net->l2, v[2], v[3], start, end);

//printf("conv 2 \n");
  conv_forward2(net->l3, v[3], v[4], start, end);

 

  relu_forward(net->l4, v[4], v[5], start, end);



  pool_forward(net->l5, v[5], v[6], start, end);


//printf("conv 3 \n");
  conv_forward3(net->l6, v[6], v[7], start, end);


  relu_forward(net->l7, v[7], v[8], start, end);



  pool_forward(net->l8, v[8], v[9], start, end);

 

  fc_forward(net->l9, v[9], v[10], start, end);



  softmax_forward(net->l10, v[10], v[11], start, end);


}

/*
 * Putting everything together: Take a set of n input images as 3-dimensional
 * Volumes and process them using the CNN in batches of 1. Then look at the
 * output (which is a set of 10 labels, each of which tells us the likelihood
 * of a specific category) and classify the image as a cat iff the likelihood
 * of "cat" is larger than 50%. Writes the cat likelihood for all images into
 * an output array (0 = definitely no cat, 1 = definitely cat).
 */


void net_classify_cats(network_t* net, vol_t** input, double* output, int n) {

 /* batch_t* batch1 = make_batch(net, 1);
   batch_t* batch2 = make_batch(net, 1);
    batch_t* batch3 = make_batch(net, 1);
     batch_t* batch4 = make_batch(net, 1);
  for (int i = 0; i < n/4*4; i+=4) {
    copy_vol(batch1[0][0], input[i]);
    net_forward(net, batch1, 0, 0);
    output[i] = batch1[11][0]->w[CAT_LABEL];

    copy_vol(batch2[0][0], input[i+1]);
    net_forward(net, batch2, 0, 0);
    output[i+1] = batch2[11][0]->w[CAT_LABEL]; 

    copy_vol(batch3[0][0], input[i+2]);
    net_forward(net, batch3, 0, 0);
    output[i+2] = batch3[11][0]->w[CAT_LABEL]; 

    copy_vol(batch4[0][0], input[i+3]);
    net_forward(net, batch4, 0, 0);
    output[i+3] = batch4[11][0]->w[CAT_LABEL]; 
  }
  for(int i = n/4*4; i< n; i++)
  {
    copy_vol(batch1[0][0], input[i]);
    net_forward(net, batch1, 0, 0);
    output[i] = batch1[11][0]->w[CAT_LABEL];


  }
  free_batch(batch1, 1);
   free_batch(batch2, 1);
    free_batch(batch3, 1);
     free_batch(batch4, 1);
*/
 
  #pragma omp parallel
  {
    #pragma omp for
  for (int i = 0; i < n; i++) {
    batch_t* batch = make_batch(net, 1);
    copy_vol(batch[0][0], input[i]);
    net_forward(net, batch, 0, 0);
    output[i] = batch[11][0]->w[3];
      free_batch(batch, 1);
  }
  }
 
   }

// IGNORE EVERYTHING BELOW THIS POINT -----------------------------------------

// Including C files in other C files is very bad style and should be avoided
// in any real application. We do it here since we want everything that you
// may edit to be in one file, without having to fix the interfaces between
// the different components of the system.

#include "util.c"
#include "main.c"