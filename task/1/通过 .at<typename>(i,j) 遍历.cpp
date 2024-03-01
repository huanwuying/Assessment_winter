void colorReduce(Mat& image,int div)  
{  
    for(int i=0;i<image.rows;i++)  
    {  
        for(int j=0;j<image.cols;j++)  
        {  
            image.at<Vec3b>(i,j)[0]=image.at<Vec3b>(i,j)[0]/div*div+div/2;  
            image.at<Vec3b>(i,j)[1]=image.at<Vec3b>(i,j)[1]/div*div+div/2;  
            image.at<Vec3b>(i,j)[2]=image.at<Vec3b>(i,j)[2]/div*div+div/2;  
        }  
    }  
}  