#include <machine_learning.h>

PersonDetection::PersonDetection() {
    model = tflite::GetModel(g_person_detect_model_data);
}

TfLiteStatus PersonDetection::init() {
    Serial.printf("Init Person detector...");
    static tflite::MicroOpResolver<3> micro_op_resolver;
    
    micro_op_resolver.AddBuiltin(
        tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
        tflite::ops::micro::Register_DEPTHWISE_CONV_2D()
    );
    
    micro_op_resolver.AddBuiltin(
        tflite::BuiltinOperator_CONV_2D,
        tflite::ops::micro::Register_CONV_2D()
    );
    
    micro_op_resolver.AddBuiltin(
        tflite::BuiltinOperator_AVERAGE_POOL_2D,
        tflite::ops::micro::Register_AVERAGE_POOL_2D()
    );

    const int kTensorArenaSize = 73 * 1024;
    static uint8_t* tensor_arena = new uint8_t[kTensorArenaSize];

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
        model, 
        micro_op_resolver, 
        tensor_arena, 
        kTensorArenaSize, 
        error_reporter
    );
    
    interpreter = &static_interpreter;

    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        Serial.println("AllocateTensors() failed");
        return allocate_status;
    }

    input = interpreter->input(0);

    Serial.println("OK");
    return allocate_status;
}

TfLiteTensor* PersonDetection::getInput() {
    return input;
}

void PersonDetection::run() {
    Serial.print("Run inference...");
    if (kTfLiteOk != interpreter->Invoke()) {
        TF_LITE_REPORT_ERROR(error_reporter,"Invoke failed.");
        Serial.println("ERROR");
        return;
    }

    TfLiteTensor* output = interpreter->output(0);

    // Process the inference results.
    uint8_t person_score = output->data.uint8[kPersonIndex];
    uint8_t no_person_score = output->data.uint8[kNotAPersonIndex];
    if(person_score > no_person_score){
        Serial.printf("Person with score %d\n", person_score);
    } else {
        Serial.printf("No Person with score %d\n", no_person_score);
    }
}