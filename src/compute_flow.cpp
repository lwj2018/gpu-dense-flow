#include<iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/cudaoptflow.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <string>
#include "ioUtils.h"

using namespace std;
using namespace cv;
using namespace cv::cuda;

clock_t start,endt;
const string video_root = "/home/haodong/Data/CSL_Isolated/color_video_125000";
const string flow_root = "/home/liweijie/NFS/Data/CSL_Isolated/flow";
const int DEBUG = 1;

uchar saturate_convert(float value,float bound)
{
    float bound_val = (value+bound)*255.0/(2*bound);
    uchar x = saturate_cast<uchar>(bound_val);
    return x;
}

void compute_TVL1(Mat prev,Mat curr,Mat& flowx,Mat& flowy,float bound=15)
{
    Ptr<OpticalFlowDual_TVL1> tvl1 = OpticalFlowDual_TVL1::create();
    clock_t ts = clock();
    GpuMat d_frame0(prev);
    GpuMat d_frame1(curr);
    GpuMat d_flow;
    clock_t te = clock();
    // if(DEBUG) cout<<"Move cost: "<<(double)(te-ts)/CLOCKS_PER_SEC*1000<<"ms"<<endl; 
    ts = clock();
    tvl1->calc(d_frame0,d_frame1,d_flow);
    te = clock();
    // if(DEBUG) cout<<"Calc cost: "<<(double)(te-ts)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
    Mat flow(d_flow);
    for (int row = 0; row < flow.rows; row++)
     {
        for (int col = 0; col < flow.cols; col++)
        {   
            float x = flow.at<Vec2f>(row, col)[0];
            float y = flow.at<Vec2f>(row, col)[1];
            flowx.at<uchar>(row,col) = saturate_convert(x,bound);
            flowy.at<uchar>(row,col) = saturate_convert(y,bound);
        }
     }
}

void save_flow(Mat flowx,Mat flowy,string flow_path,int index)
{
    char flowx_name[500];
    char flowy_name[500];
    sprintf(flowx_name,(flow_path+"/x_%06d.jpg").c_str(),index);
    sprintf(flowy_name,(flow_path+"/y_%06d.jpg").c_str(),index);
    imwrite(flowx_name,flowx);
    imwrite(flowy_name,flowy);
}

void cal_for_frames(string video_path)
{
    cout<<video_path<<endl;
    std::vector<string> filename_list;
    string format = ".jpg";
    readFileList(filename_list,video_path.c_str(),format);
    sort(filename_list.begin(),filename_list.end());
    // create flow path
    vector<string> splits;
    StringSplit(video_path,'/',splits);
    string flow_path = flow_root+"/"+splits[splits.size()-2]+\
        +"/"+splits[splits.size()-1];
    create_path(flow_path);
    // Read the first frame
    Mat raw = imread(filename_list[0]);
    // Create containers
    Mat prev;
    Mat curr;
    Mat flowx(raw.size(),CV_8UC1);
    Mat flowy(raw.size(),CV_8UC1);
    // Convert to gray
    cvtColor(raw,prev,CV_BGR2GRAY);
    start = clock();
    for (int i = 1; i < filename_list.size(); i++)
    {
        clock_t ts = clock();
        raw = imread(filename_list[i]);
        cvtColor(raw,curr,CV_BGR2GRAY);
        compute_TVL1(prev,curr,flowx,flowy);
        clock_t te = clock();
        // if(DEBUG) cout<<"Calc cost: "<<(double)(te-ts)/CLOCKS_PER_SEC*1000<<"ms"<<endl; 
        ts = clock();
        save_flow(flowx,flowy,flow_path,i);
        te = clock();
        prev = curr.clone();
    }
    endt = clock();
    double endtime=(double)(endt-start)/CLOCKS_PER_SEC;
	cout<<"Total time: "<<endtime<<"s"<<endl;	//ms为单位
}

int main()
{
    std::vector<string> folder_list;
    findAllSubDir(folder_list, video_root.c_str(), false);
    sort(folder_list.begin(),folder_list.end());
    for (int i = 0; i < folder_list.size(); i++)
    {
        string folder = folder_list[i];
        std::vector<string> subfolder_list;
        findAllSubDir(subfolder_list, folder.c_str(), false);
        sort(subfolder_list.begin(),subfolder_list.end());
        for (int j = 0; j < subfolder_list.size(); j++)
        {
            string video_path = subfolder_list[j];
            cal_for_frames(video_path);
        }
        
    }
        
}