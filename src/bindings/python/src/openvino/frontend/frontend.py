# -*- coding: utf-8 -*-
# Copyright (C) 2018-2023 Intel Corporation
# SPDX-License-Identifier: Apache-2.0

from typing import Union

from openvino._pyopenvino import FrontEnd as FrontEndBase
from openvino._pyopenvino import FrontEndManager as FrontEndManagerBase
from openvino._pyopenvino import InputModel
from openvino.runtime import Model


class FrontEnd(FrontEndBase):
    def __init__(self, fe: FrontEndBase) -> None:
        super().__init__(fe)

    def convert(self, model: Union[Model, InputModel]) -> Model:
        converted_model = super().convert(model)
        if isinstance(model, InputModel):
            return Model(converted_model)
        return converted_model

    def convert_partially(self, model: InputModel) -> Model:
        return Model(super().convert_partially(model))

    def decode(self, model: InputModel) -> Model:
        return Model(super().decode(model))

    def normalize(self, model: Model) -> Model:
        return Model(super().normalize(model))


class FrontEndManager(FrontEndManagerBase):
    def load_by_framework(self, framework: str) -> Union[FrontEnd, None]:
        fe = super().load_by_framework(framework)
        if fe is not None:
            return FrontEnd(fe)
        return fe

    def load_by_model(self, model_path: str) -> Union[FrontEnd, None]:
        fe = super().load_by_model(model_path)
        if fe is not None:
            return FrontEnd(fe)
        return fe
