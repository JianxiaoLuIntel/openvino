// Copyright (C) 2018-2025 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "openvino/op/log.hpp"

#include "element_visitor.hpp"
#include "itt.hpp"
#include "openvino/reference/log.hpp"

namespace ov {
namespace op {
namespace log {
struct Evaluate : element::NoAction<bool> {
    using element::NoAction<bool>::visit;

    template <element::Type_t ET, class T = fundamental_type_for<ET>>
    static result_type visit(const Tensor& in, Tensor& out, const size_t count) {
        reference::log(in.data<const T>(), out.data<T>(), count);
        return true;
    }
};
}  // namespace log

namespace v0 {
Log::Log(const Output<Node>& arg) : UnaryElementwiseArithmetic(arg) {
    constructor_validate_and_infer_types();
}

std::shared_ptr<Node> Log::clone_with_new_inputs(const OutputVector& new_args) const {
    OV_OP_SCOPE(v0_Log_clone_with_new_inputs);
    check_new_args_count(this, new_args);
    return std::make_shared<Log>(new_args.at(0));
}

bool Log::evaluate(TensorVector& outputs, const TensorVector& inputs) const {
    OV_OP_SCOPE(v0_Log_evaluate);
    OPENVINO_ASSERT(outputs.size() == 1);
    OPENVINO_ASSERT(inputs.size() == 1);

    const auto& input_shape = inputs[0].get_shape();
    const auto count = shape_size(input_shape);
    outputs[0].set_shape(input_shape);
    using namespace ov::element;
    return IF_TYPE_OF_CONVERT_TENSORS(v0_Log_evaluate,
                                      this,
                                      outputs,
                                      inputs,
                                      OV_PP_ET_LIST(f32, i32, i64, u32, u64),
                                      log::Evaluate,
                                      inputs[0].get_element_type(),
                                      inputs[0],
                                      outputs[0],
                                      count);
}

bool Log::has_evaluate() const {
    OV_OP_SCOPE(v0_Log_has_evaluate);
    switch (get_input_element_type(0)) {
    case element::f16:
    case element::f32:
    case element::i32:
    case element::i64:
    case element::u32:
    case element::u64:
        return true;
    default:
        return false;
    }
}
}  // namespace v0
}  // namespace op
}  // namespace ov
