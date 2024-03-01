void colorReduce(const Mat& image,Mat& outImage,int div)  
{  
    // 创建与原图像等尺寸的图像  
    outImage.create(image.size(),image.type());  
    int nr=image.rows;  
    // 将3通道转换为1通道  
    int nl=image.cols*image.channels();  
    for(int k=0;k<nr;k++)  
    {  
        // 每一行图像的指针  
        const uchar* inData=image.ptr<uchar>(k);  
        uchar* outData=outImage.ptr<uchar>(k);  
        for(int i=0;i<nl;i++)  
        {  
            outData[i]=inData[i]/div*div+div/2;  
        }  
    }  
} 