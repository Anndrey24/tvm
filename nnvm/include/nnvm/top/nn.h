/*!
 *  Copyright (c) 2017 by Contributors
 * \file nnvm/top/nn.h
 * \brief Auxiliary param for tensor primitive.
 */
#ifndef NNVM_TOP_NN_H_
#define NNVM_TOP_NN_H_

#include <dmlc/base.h>
#include <dmlc/parameter.h>
#include <nnvm/tuple.h>
#include <nnvm/layout.h>
#include <string>

namespace nnvm {
namespace top {

struct DenseParam : public dmlc::Parameter<DenseParam> {
  int units;
  bool use_bias;

  DMLC_DECLARE_PARAMETER(DenseParam) {
    DMLC_DECLARE_FIELD(units).set_lower_bound(1)
    .describe("Number of hidden units of the dense transformation.");
    DMLC_DECLARE_FIELD(use_bias).set_default(true)
    .describe("Whether to use bias parameter");
  }
  // constants
  static const constexpr int kData = 0;
  static const constexpr int kWeight = 1;
  static const constexpr int kBias = 2;
};

struct DropoutParam : public dmlc::Parameter<DropoutParam> {
  float rate;

  DMLC_DECLARE_PARAMETER(DropoutParam) {
    DMLC_DECLARE_FIELD(rate).set_default(0.5)
        .set_range(0, 1)
        .describe("Fraction of the input that gets dropped out during training time.");
  }
};

struct BatchNormParam : public dmlc::Parameter<BatchNormParam> {
  int axis;
  double epsilon;
  double momentum;
  bool center;
  bool scale;

  DMLC_DECLARE_PARAMETER(BatchNormParam) {
    DMLC_DECLARE_FIELD(axis).set_default(1)
      .describe("Specify which shape axis the channel is specified.");
    DMLC_DECLARE_FIELD(epsilon).set_default(1e-5)
        .describe("Small float added to variance to avoid dividing by zero.");
    DMLC_DECLARE_FIELD(center).set_default(true)
        .describe("If True, add offset of `beta` to normalized tensor."
                  "If False, `beta` is ignored.");
    DMLC_DECLARE_FIELD(scale).set_default(true)
        .describe("If True, multiply by `gamma`. If False, `gamma` is not used."
                  "When the next layer is piecewise linear (also e.g. `nn.relu`),"
                  "this can be disabled since the scaling"
                  "will be done by the next layer.");
  }
  // constants
  static const constexpr int kData = 0;
  static const constexpr int kGamma = 1;
  static const constexpr int kBeta = 2;
  static const constexpr int kMovingMean = 3;
  static const constexpr int kMovingVariance = 4;
};


// Shared by softmax and log_softmax
struct SoftmaxParam : public dmlc::Parameter<SoftmaxParam> {
  int axis;

  DMLC_DECLARE_PARAMETER(SoftmaxParam) {
    DMLC_DECLARE_FIELD(axis).set_default(-1)
        .describe("The axis to sum over when computing softmax.");
  }
};

struct LeakyReLUParam : public dmlc::Parameter<LeakyReLUParam> {
  double alpha;

  DMLC_DECLARE_PARAMETER(LeakyReLUParam) {
    DMLC_DECLARE_FIELD(alpha).set_lower_bound(0.0).set_default(0.25)
        .describe("slope coefficient for the negative half axis.");
  }
};

struct PReLUParam : public dmlc::Parameter<PReLUParam> {
  int axis;
  DMLC_DECLARE_PARAMETER(PReLUParam) {
    DMLC_DECLARE_FIELD(axis).set_default(1)
      .describe("Specify which shape axis the channel is specified.");
  }
};

struct PadParam : public dmlc::Parameter<PadParam> {
  float pad_value;
  Tuple<Tuple<int> > pad_width;

  DMLC_DECLARE_PARAMETER(PadParam) {
    DMLC_DECLARE_FIELD(pad_value).set_default(0.0)
      .describe("The value to be padded.");
    DMLC_DECLARE_FIELD(pad_width)
      .describe("Number of values padded to the edges of each axis, "
                "in the format of ((before_1, after_1), ... (before_N, after_N))");
  }
};


struct Conv2DParam : public dmlc::Parameter<Conv2DParam> {
  int channels;
  TShape kernel_size;
  TShape strides;
  TShape padding;
  TShape dilation;
  int groups;
  std::string layout;
  std::string kernel_layout;
  std::string out_layout;
  bool use_bias;

  DMLC_DECLARE_PARAMETER(Conv2DParam) {
    DMLC_DECLARE_FIELD(channels)
      .describe("The dimensionality of the output space"
                "i.e. the number of output channels in the convolution.");
    DMLC_DECLARE_FIELD(kernel_size)
      .describe("Specifies the dimensions of the convolution window.");
    DMLC_DECLARE_FIELD(strides).set_default(TShape({1, 1}))
      .describe("Specifies the strides of the convolution.");
    DMLC_DECLARE_FIELD(padding).set_default(TShape({0, 0}))
      .describe("If padding is non-zero, then the input is implicitly zero-padded"
                "on both sides for padding number of points");
    DMLC_DECLARE_FIELD(dilation).set_default(TShape({1, 1}))
      .describe("Specifies the dilation rate to use for dilated convolution.");
    DMLC_DECLARE_FIELD(groups).set_default(1)
      .describe("Controls the connections between inputs and outputs."
                "At groups=1, all inputs are convolved to all outputs."
                "At groups=2, the operation becomes equivalent to having two convolution"
                "layers side by side, each seeing half the input channels, and producing"
                "half the output channels, and both subsequently concatenated.");
    DMLC_DECLARE_FIELD(layout).set_default("NCHW")
      .describe("Dimension ordering of input data. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Convolution is applied on the 'H' and"
                "'W' dimensions.");
    DMLC_DECLARE_FIELD(out_layout).set_default("__undef__")
      .describe("Dimension ordering of output. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Default to be same as input layout.");
    DMLC_DECLARE_FIELD(kernel_layout).set_default("OIHW")
      .describe("Dimension ordering of weight. Can be 'OIHW', 'OIHW16o16i', etc."
                "'O', 'I', 'H', 'W' stands for num_filter, input_channel, height, and width"
                "dimensions respectively.");
    DMLC_DECLARE_FIELD(use_bias).set_default(true)
      .describe("Whether the layer uses a bias vector.");
  }
  // constants
  static const constexpr int kData = 0;
  static const constexpr int kWeight = 1;
  static const constexpr int kBias = 2;
};


struct Conv2DTransposeParam : public dmlc::Parameter<Conv2DTransposeParam> {
  int channels;
  TShape kernel_size;
  TShape strides;
  TShape padding;
  TShape output_padding;
  TShape dilation;
  int groups;
  std::string layout;
  std::string kernel_layout;
  bool use_bias;

  DMLC_DECLARE_PARAMETER(Conv2DTransposeParam) {
    DMLC_DECLARE_FIELD(channels)
      .describe("The dimensionality of the output space"
                "i.e. the number of output channels in the convolution.");
    DMLC_DECLARE_FIELD(kernel_size)
      .describe("Specifies the dimensions of the convolution window.");
    DMLC_DECLARE_FIELD(strides).set_default(TShape({1, 1}))
      .describe("Specifies the strides of the convolution.");
    DMLC_DECLARE_FIELD(output_padding).set_default(TShape({0, 0}))
      .describe("Zero-padding added to one side of the output.");
    DMLC_DECLARE_FIELD(padding).set_default(TShape({0, 0}))
      .describe("If padding is non-zero, then the input is implicitly zero-padded"
                "on both sides for padding number of points");
    DMLC_DECLARE_FIELD(dilation).set_default(TShape({1, 1}))
      .describe("Specifies the dilation rate to use for dilated convolution.");
    DMLC_DECLARE_FIELD(groups).set_default(1)
      .describe("Controls the connections between inputs and outputs."
                "At groups=1, all inputs are convolved to all outputs."
                "At groups=2, the operation becomes equivalent to having two convolution"
                "layers side by side, each seeing half the input channels, and producing"
                "half the output channels, and both subsequently concatenated.");
    DMLC_DECLARE_FIELD(layout).set_default("NCHW")
      .describe("Dimension ordering of data. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Convolution is applied on the 'H' and"
                "'W' dimensions.");
    DMLC_DECLARE_FIELD(kernel_layout).set_default("OIHW")
      .describe("Dimension ordering of data and weight. Can be 'OIHW', 'OIHW16o16i', etc."
                "'O', 'I', 'H', 'W' stands for num_filter, input_channel, height, and width"
                "dimensions respectively.");
    DMLC_DECLARE_FIELD(use_bias).set_default(true)
      .describe("Whether the layer uses a bias vector.");
  }
  // constants
  static const constexpr int kData = 0;
  static const constexpr int kWeight = 1;
  static const constexpr int kBias = 2;
};


struct MaxPool2DParam : public dmlc::Parameter<MaxPool2DParam> {
  TShape pool_size;
  TShape strides;
  TShape padding;
  std::string layout;
  bool ceil_mode;

  DMLC_DECLARE_PARAMETER(MaxPool2DParam) {
    DMLC_DECLARE_FIELD(pool_size)
      .describe("Size of the pooling windows..");
    DMLC_DECLARE_FIELD(strides).set_default(TShape({1, 1}))
      .describe("Specifies the strides of the convolution.");
    DMLC_DECLARE_FIELD(padding).set_default(TShape({0, 0}))
      .describe("If padding is non-zero, then the input is implicitly zero-padded"
                "on both sides for padding number of points");
    DMLC_DECLARE_FIELD(layout).set_default("NCHW")
      .describe("Dimension ordering of data and weight. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Convolution is applied on the 'H' and"
                "'W' dimensions.");
    DMLC_DECLARE_FIELD(ceil_mode).set_default(false)
      .describe("When true, will use ceil instead of floor to compute the output shape.");
  }
};


struct AvgPool2DParam : public dmlc::Parameter<AvgPool2DParam> {
  TShape pool_size;
  TShape strides;
  TShape padding;
  std::string layout;
  bool ceil_mode;
  bool count_include_pad;

  DMLC_DECLARE_PARAMETER(AvgPool2DParam) {
    DMLC_DECLARE_FIELD(pool_size)
      .describe("Size of the pooling windows..");
    DMLC_DECLARE_FIELD(strides).set_default(TShape({1, 1}))
      .describe("Specifies the strides of the convolution.");
    DMLC_DECLARE_FIELD(padding).set_default(TShape({0, 0}))
      .describe("If padding is non-zero, then the input is implicitly zero-padded"
                "on both sides for padding number of points");
    DMLC_DECLARE_FIELD(layout).set_default("NCHW")
      .describe("Dimension ordering of data and weight. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Convolution is applied on the 'H' and"
                "'W' dimensions.");
    DMLC_DECLARE_FIELD(ceil_mode).set_default(false)
      .describe("When true, will use ceil instead of floor to compute the output shape.");
    DMLC_DECLARE_FIELD(count_include_pad).set_default(false)
      .describe("When true, will include padding to compute the average");
  }
};


struct GlobalPool2DParam : public dmlc::Parameter<GlobalPool2DParam> {
  std::string layout;

  DMLC_DECLARE_PARAMETER(GlobalPool2DParam) {
    DMLC_DECLARE_FIELD(layout).set_default("NCHW")
      .describe("Dimension ordering of data and weight. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Convolution is applied on the 'H' and"
                "'W' dimensions.");
  }
};

struct UpSamplingParam : public dmlc::Parameter<UpSamplingParam> {
  int scale;
  std::string layout;

  DMLC_DECLARE_PARAMETER(UpSamplingParam) {
    DMLC_DECLARE_FIELD(scale)
      .describe("upsampling scaling factor");
    DMLC_DECLARE_FIELD(layout)
      .set_default("NCHW")
      .describe("Dimension ordering of data and weight. Can be 'NCHW', 'NHWC', etc."
                "'N', 'C', 'H', 'W' stands for batch, channel, height, and width"
                "dimensions respectively. Convolution is applied on the 'H' and"
                "'W' dimensions.");
  }
};

struct LayoutTransformParam : public dmlc::Parameter<LayoutTransformParam> {
  std::string src_layout;
  std::string dst_layout;

  DMLC_DECLARE_PARAMETER(LayoutTransformParam) {
    DMLC_DECLARE_FIELD(src_layout).set_default("__undef__")
    .describe("Dimension ordering of data");
    DMLC_DECLARE_FIELD(dst_layout).set_default("__undef__")
    .describe("Dimension ordering of data.");
  }
};

}  // namespace top
}  // namespace nnvm

#endif  // NNVM_TOP_NN_H_
