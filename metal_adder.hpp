//
//  metal_adder.hpp
//  try
//
//  Created by Moritz Hof on 03.01.22.
//

#ifndef metal_adder_hpp
#define metal_adder_hpp

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

class metal_adder {
public:
  void init_with_device(MTL::Device *device);

  void prepare_data();

  void send_compute_command();

  static void random_number_generator(MTL::Buffer *buffer);

  void encode_add_command(MTL::ComputeCommandEncoder *compute_encoder);

  static unsigned int get_size();

  void verify();

private:
  MTL::Buffer *_A;
  MTL::Buffer *_B;
  MTL::Buffer *_C;

  MTL::Device *_device;
  MTL::CommandQueue *_CommandQueue;
  MTL::ComputePipelineState *_addFunctionPSO;

  static void zero_buffer(MTL::Buffer *pBuffer);
};

#endif /* metal_adder_hpp */
