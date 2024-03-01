void colorReduce(const Mat& image,Mat& outImage,int div)  
{  
    outImage.create(image.size(),image.type());
    // 获得迭代器  
    MatConstIterator_<Vec3b> it_in=image.begin<Vec3b>();  
    MatConstIterator_<Vec3b> itend_in=image.end<Vec3b>();  
    MatIterator_<Vec3b> it_out=outImage.begin<Vec3b>();  
    MatIterator_<Vec3b> itend_out=outImage.end<Vec3b>();  
    while(it_in!=itend_in)  
    {  
        (*it_out)[0]=(*it_in)[0]/div*div+div/2;  
        (*it_out)[1]=(*it_in)[1]/div*div+div/2;  
        (*it_out)[2]=(*it_in)[2]/div*div+div/2;  
        it_in++;  
        it_out++;  
    }  
} 