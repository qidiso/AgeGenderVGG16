#include <cuda_runtime.h>

#include <cstring>
#include <cstdlib>
#include <vector>

#include <string>
#include <iostream>
#include <stdio.h>
#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include "caffe/blob.hpp"

using namespace caffe;
using namespace std;

int main(int argc, char** argv) {
 
 /* if (argc < 4 || argc > 6) {
    LOG(ERROR) << "test_net net_proto pretrained_net_proto iterations "
        << "[CPU/GPU] [Device ID]";
    return 1;
  }
  //Caffe::set_phase(Caffe::TEST);

  //Setting CPU or GPU
  if (argc >= 5 && strcmp(argv[4], "GPU") == 0) {
    Caffe::set_mode(Caffe::GPU);
    int device_id = 0;
    if (argc == 6) {
      device_id = atoi(argv[5]);
    }
    Caffe::SetDevice(device_id);
    LOG(ERROR) << "Using GPU #" << device_id;
  } else {
    LOG(ERROR) << "Using CPU";
    Caffe::set_mode(Caffe::CPU);
  }*/

  Caffe::set_mode(Caffe::GPU);
  int device_id = 0;
  Caffe::SetDevice(device_id);
  LOG(INFO) << "Using GPU";

  //get the net
  //Net<float> caffe_test_net(argv[1]);
  Net<float> caffe_test_net("./age.prototxt",caffe::TEST);
 
  //get trained net
  //caffe_test_net.CopyTrainedLayersFrom(argv[2]);
  caffe_test_net.CopyTrainedLayersFrom("./dex_chalearn_iccv2015.caffemodel");

  //get the net
  //Net<float> caffe_test_net(argv[1]);
   Net<float> caffe_test2_net("./age2.prototxt",caffe::TEST);
 
  //get trained net
  //caffe_test_net.CopyTrainedLayersFrom(argv[2]);
  caffe_test2_net.CopyTrainedLayersFrom("./dex_imdb_wiki.caffemodel");

    //get the net
  //Net<float> caffe_test_net(argv[1]);
   Net<float> caffe_test3_net("./gender.prototxt",caffe::TEST);
 
  //get trained net
  //caffe_test_net.CopyTrainedLayersFrom(argv[2]);
  caffe_test3_net.CopyTrainedLayersFrom("./gender.caffemodel");


  
  //get datum
  Datum datum;
  if (!ReadImageToDatum("./example.jpg", 1, 224, 224, &datum)) {
    LOG(ERROR) << "Error during file reading";
  }

  //get the blob
  Blob<float>* blob = new Blob<float>(1, datum.channels(), datum.height(), datum.width());
  
  //get the blobproto
  BlobProto blob_proto;
  blob_proto.set_num(1);
  blob_proto.set_channels(datum.channels());
  blob_proto.set_height(datum.height());
  blob_proto.set_width(datum.width());

  
  const int data_size = datum.channels() * datum.height() * datum.width();
  int size_in_datum = std::max<int>(datum.data().size(),
                                    datum.float_data_size());
  for (int i = 0; i < size_in_datum; ++i) {
    blob_proto.add_data(0.);
  }
  const string& data = datum.data();
  if (data.size() != 0) {
    for (int i = 0; i < size_in_datum; ++i) {
      blob_proto.set_data(i, blob_proto.data(i) + (uint8_t)data[i]);
    }
  }
  
  //set data into blob
  blob->FromProto(blob_proto);

  //fill the vector
  vector<Blob<float>*> bottom;
  bottom.push_back(blob);
  float type = 0.0;

  const vector<Blob<float>*>& result =  caffe_test_net.Forward(bottom, &type);

  //Here I can use the argmax layer, but for now I do a simple for :)
  float max = 0;
  float max_i = 0;
  for (int i = 0; i < 101; ++i) {
    float value = result[0]->cpu_data()[i];
    if (max < value){
      max = value;
      max_i = i;
    }
  }
  LOG(ERROR) << "max: " << max << " i " << max_i;


const vector<Blob<float>*>& result2 =  caffe_test2_net.Forward(bottom, &type);

  //Here I can use the argmax layer, but for now I do a simple for :)
max = 0;
max_i = 0;

  for (int i = 0; i < 101; ++i) {
    float value = result2[0]->cpu_data()[i];
    if (max < value){
      max = value;
      max_i = i;
    }
  }
  LOG(ERROR) << "max: " << max << " i " << max_i;


  const vector<Blob<float>*>& result3 =  caffe_test3_net.Forward(bottom, &type);

  //Here I can use the argmax layer, but for now I do a simple for :)
max = 0;
max_i = 0;

  for (int i = 0; i < 2; ++i) {
    float value = result3[0]->cpu_data()[i];
    if (max < value){
      max = value;
      max_i = i;
    }
  }
  if(max_i==1)
  {
    LOG(ERROR) << "max: " << max << " i " << max_i << " Male";
  }
  else
  {
    LOG(ERROR) << "max: " << max << " i " << max_i << " Female";
  }
  



  return 0;
}
