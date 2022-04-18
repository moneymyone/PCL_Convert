/*
 * @Description: 
 * @version: 1.0.0
 * @Author: qianchengjun
 * @Date: 2022-02-22 14:33:45
 * @LastEditors: qianchengjun
 * @LastEditTime: 2022-02-22 16:49:34
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
using namespace std;

void getFiles(const char* path, vector<string>& files){

    const string path0 = path;
    DIR* pDir;
    struct dirent* ptr;

    struct stat s;
    lstat(path, &s);

    if(!S_ISDIR(s.st_mode)){
        cout << "not a valid directory: " << path << endl;
        return;
    }

    if(!(pDir = opendir(path))){
        cout << "opendir error: " << path << endl;
        return;
    }
    int i = 0;
    string subFile;
    while((ptr = readdir(pDir)) != 0){
        subFile = ptr -> d_name;
        if(subFile == "." || subFile == "..")
            continue;
        subFile = path0 + subFile;
        cout << ++i << ": " << subFile << endl;
        files.push_back(subFile);
    }
    closedir(pDir);

}


struct MyPointType    //定义点类型结构
{
    PCL_ADD_POINT4D;  //该点类型有4个元素      

    /*尝试新增一个自定义*/  
    unsigned m;    //定义自己新增的类型名称
     //测试了添加这么多个也没问题
    float intensity;
    unsigned k;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW   //确保new操作符对齐操作 

}EIGEN_ALIGN16;   //强制SSE 对齐

POINT_CLOUD_REGISTER_POINT_STRUCT(MyPointType,    //注册点类型宏
    (float ,x,x)
    (float ,y,y)
    (float ,z,z)
    (unsigned char ,m,m)
    (float ,intensity,intensity)
    (unsigned ,k,k)
    )

void ConvertPCD(std::string file)
{
    pcl::PointCloud<MyPointType> cloudsrc;
    pcl::PointCloud<pcl::PointXYZI> cloudtar;

    if (pcl::io::loadPCDFile<MyPointType>(file, cloudsrc) == -1)
    {
        PCL_ERROR("Couldn'n read file! \n");
        return;
    }
    
    std::cout << cloudsrc.points.size() << std::endl;

    for (size_t i = 0; i < cloudsrc.points.size(); i++)
    {
        pcl::PointXYZI pt;
        pt.x = cloudsrc[i].x;
        pt.y = cloudsrc[i].y;
        pt.z = cloudsrc[i].z;
        pt.intensity = cloudsrc[i].intensity;
        cloudtar.push_back(pt);
    }
    std::string newfile = file.replace(file.rfind(".pcd"),4,"_new.pcd");
    pcl::PCDWriter writer;
	writer.write<pcl::PointXYZI> (newfile, cloudtar, true); // 保存文件
}

int main(int argc, char** argv)
{
    //目标文件夹路径
    std::string folder = "/home/qiancj/Documents/M1/Underground_Garage/4.5mleft_head/";
    vector<string> files;
    getFiles(folder.c_str(), files);
    for_each(files.begin(), files.end(), [](const string &file){
        ConvertPCD(file);
        cout << "finish convert file: " << file << endl; });

    return 0;
}