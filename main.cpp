#include <iostream>

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include "metal_adder.hpp"

int main() {
  auto p_pool = NS::AutoreleasePool::alloc()->init();
  auto device = MTL::CreateSystemDefaultDevice();

  auto adder = new metal_adder();
  adder->init_with_device(device);
  adder->prepare_data();
  adder->send_compute_command();
  adder->verify();
  delete adder;

  std::cout << " End of Computation  " << std::endl;
  p_pool->release();
  std::cout << "Done" << std::endl;
  return 0;
}
