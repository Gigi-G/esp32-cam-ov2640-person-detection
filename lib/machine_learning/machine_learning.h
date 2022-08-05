#ifndef MACHINE_LEARNING
#define MACHINE_LEARNING

#include <TensorFlowLite.h>
#include <Arduino.h>

#include "model_settings.h"
#include "person_detect_model_data.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

class PersonDetection {
    private:
        tflite::ErrorReporter* error_reporter = nullptr;
        const tflite::Model* model = nullptr;
        tflite::MicroInterpreter* interpreter = nullptr;
        TfLiteTensor* input = nullptr;
    
    public:
        PersonDetection();
        TfLiteStatus init();
        TfLiteTensor* getInput();
        void run();
};

#endif