
__kernel void add(__global float *tgt, __global const float *op1, __global const float *op2, const int num)
{
  int idx = get_local_id(0);
  if(idx < num)
  	tgt[idx] = op1[idx] + op2[idx];
}
