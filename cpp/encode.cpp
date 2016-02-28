#include <iostream>
#include <stdint.h>

#include "codecfactory.h"
#include "intersection.h"
#include "variablebyte.h"

using namespace SIMDCompressionLib;

// sorted
static shared_ptr<IntegerCODEC> codec_sorted =  CODECFactory::getFromName("s4-bp128-dm");
// variable byte
static VariableByte<false> codec_unsorted = VariableByte<false>();

static SIMDBinaryPacking<SIMDIntegratedBlockPacker<Max4DeltaSIMD, true>> codec_packed_sorted = SIMDBinaryPacking<SIMDIntegratedBlockPacker<Max4DeltaSIMD, true>>();

extern "C" {


  size_t encode_sorted_native(
       uint32_t* begin,
       const size_t num_els,
       uint32_t* output,
       const size_t output_capacity) {
        size_t output_length = output_capacity;
        codec_sorted -> encodeArray(begin,
                          num_els,
                          output,
                          output_length);
        return output_length;
  }

  size_t encode_unsorted_native(
       uint32_t* begin,
       const size_t num_els,
       uint32_t* output,
       const size_t output_capacity) {
         size_t output_length = output_capacity;
         codec_unsorted.encodeArray(begin,
                           num_els,
                           output,
                           output_length);
        return output_length;
  }

  size_t decode_sorted_native(
      const uint32_t* compressed_data,
      const size_t compressed_size,
      uint32_t* uncompressed,
      const size_t uncompressed_capacity) {
        size_t num_ints = uncompressed_capacity;
        codec_sorted -> decodeArray(compressed_data, compressed_size, uncompressed, num_ints);
        return num_ints;
  }

  size_t decode_unsorted_native(
      const uint32_t* compressed_data,
      const size_t compressed_size,
      uint32_t* uncompressed,
      const size_t uncompressed_capacity) {
         size_t num_ints = uncompressed_capacity;
         codec_unsorted.decodeArray(compressed_data, compressed_size, uncompressed, num_ints);
         return num_ints;
  }

  size_t intersection_native(
      const uint32_t* left,
      const size_t left_size,
      const uint32_t* right,
      const size_t right_size,
      uint32_t* output) {
        return IntersectionFactory::getFromName("simd")(left, left_size, right, right_size, output);
  }

}
