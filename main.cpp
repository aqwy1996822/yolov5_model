#include "yolo/yolov5_model.h"

bool parse_args(int argc, char** argv, std::string& wts, std::string& engine, bool& is_p6, float& gd, float& gw, std::string& img_dir) {

    if (std::string(argv[1]) == "-s" && (argc == 5 || argc == 7)) {
        wts = std::string(argv[2]);
        engine = std::string(argv[3]);
        auto net = std::string(argv[4]);
        if (net[0] == 'n') {
            gd = 0.33;
            gw = 0.25;
        } else if (net[0] == 's') {
            gd = 0.33;
            gw = 0.50;
        } else if (net[0] == 'm') {
            gd = 0.67;
            gw = 0.75;
        } else if (net[0] == 'l') {
            gd = 1.0;
            gw = 1.0;
        } else if (net[0] == 'x') {
            gd = 1.33;
            gw = 1.25;
        } else if (net[0] == 'c' && argc == 7) {
            gd = atof(argv[5]);
            gw = atof(argv[6]);
        } else {
            return false;
        }
        if (net.size() == 2 && net[1] == '6') {
            is_p6 = true;
        }
    } else if (std::string(argv[1]) == "-d" && argc == 4) {
        engine = std::string(argv[2]);
        img_dir = std::string(argv[3]);
    } else {
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    Yolov5_Model yolov5model;

    std::string wts_name = "";
    std::string engine_name = "";
    bool is_p6 = false;
    float gd = 0.0f, gw = 0.0f;
    std::string img_dir;
    if (!parse_args(argc, argv, wts_name, engine_name, is_p6, gd, gw, img_dir)) {
        std::cerr << "arguments not right!" << std::endl;
        std::cerr << "./yolov5 -s [.wts] [.engine] [n/s/m/l/x/n6/s6/m6/l6/x6 or c/c6 gd gw]  // serialize model to plan file" << std::endl;
        std::cerr << "./yolov5 -d [.engine] ../samples  // deserialize plan file and run inference" << std::endl;
        return -1;
    }
    cv::Mat img = cv::imread("../testimgs/COCO_train2014_000000012156.jpg");
    yolov5model.deserialize_engine("../yolov5s_int8.engine");
    auto res = yolov5model.run_frame(img);
    cv::imshow("img", img);
    cv::waitKey(100);
    cv::VideoCapture cap(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    while (cap.isOpened())
    {
        cap.read(img);
        auto res = yolov5model.run_frame(img);
        cv::imshow("img", img);
        cv::waitKey(1);
    }
    cap.release();

//    yolov5model.int8_flag=true;
//    yolov5model.build(wts_name, engine_name, is_p6, gd, gw, img_dir);

    return 0;
}