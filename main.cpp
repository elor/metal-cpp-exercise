#include <stdexcept>
#include <iostream>
#include <random>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

using namespace std;

const int BUFFER_SIZE = 1024;

class MetalRaytracer {
public:
    MTL::Device *device;
    MTL::ComputePipelineState *pipeline_state;
    MTL::CommandQueue *command_queue;

    MTL::Buffer *input_buffer_A;
    MTL::Buffer *input_buffer_B;
    MTL::Buffer *output_buffer;

    void init_with_device(MTL::Device *_device) {
      device = _device;
      NS::Error *error{};
      auto filePath = NS::String::string("/Users/elor/Code/metal-cpp-test/main.metallib", NS::ASCIIStringEncoding);
      auto library = device->newLibrary(filePath, &error);

      if (!library) {
        std::cout << error->code() << std::endl;
        std::cout << error->domain()->cString(NS::ASCIIStringEncoding) << std::endl;
        std::cout << error->localizedDescription()->cString(NS::ASCIIStringEncoding) << std::endl;
        throw std::runtime_error("Failed to create default library");
      }

      auto function_name = NS::String::string("add", NS::ASCIIStringEncoding);
      auto add_function = library->newFunction(function_name);

      if (!add_function)
        throw std::runtime_error("Failed to create add function");

      pipeline_state = device->newComputePipelineState(add_function, &error);
      command_queue = device->newCommandQueue();
    }

    void prepare_data() {
      input_buffer_A = device->newBuffer(BUFFER_SIZE, MTL::StorageModeShared);
      generate_random_data(input_buffer_A);

      input_buffer_B = device->newBuffer(BUFFER_SIZE, MTL::StorageModeShared);
      generate_random_data(input_buffer_B);

      output_buffer = device->newBuffer(BUFFER_SIZE, MTL::ResourceStorageModeShared);
      fill_zeroes(output_buffer);
    }

    static void generate_random_data(MTL::Buffer *buffer) {
      static std::random_device rd;
      static std::mt19937 gen(rd());
      static std::uniform_real_distribution<float> random_uniform(0.0, 1.0);

      auto *data = reinterpret_cast<float *>(buffer->contents());
      for (int i = 0; i < BUFFER_SIZE; i++) {
        data[i] = random_uniform(gen);
      }
    }

    static void fill_zeroes(MTL::Buffer *buffer) {
      auto *data = reinterpret_cast<float *>(buffer->contents());
      for (int i = 0; i < BUFFER_SIZE; i++) {
        data[i] = 0.0;
      }
    }

    void send_compute_command() const {
      auto command_buffer = command_queue->commandBuffer();
      auto compute_encoder = command_buffer->computeCommandEncoder();
      encode_add_command(compute_encoder);
      compute_encoder->endEncoding();
      command_buffer->commit();
      command_buffer->waitUntilCompleted();
    }

    void encode_add_command(MTL::ComputeCommandEncoder *encoder) const {
      encoder->setComputePipelineState(pipeline_state);
      encoder->setBuffer(input_buffer_A, 0, 0);
      encoder->setBuffer(input_buffer_B, 0, 1);
      encoder->setBuffer(output_buffer, 0, 2);
      const MTL::Size &grid = MTL::Size{BUFFER_SIZE, 1, 1};
      const MTL::Size &threadgroup = MTL::Size{1, 1, 1};
      encoder->dispatchThreadgroups(grid, threadgroup);
    }
};

int main() {
  auto pool = NS::AutoreleasePool::alloc()->init();
  auto device = MTL::CreateSystemDefaultDevice();
  auto raytracer = new MetalRaytracer{};

  raytracer->init_with_device(device);
  raytracer->prepare_data();
  raytracer->send_compute_command();

  pool->release();

  std::cout << "Done" << std::endl;

  return 0;
}
