#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <string>
#import <iostream>
#import "Interfaces.hpp"

using namespace std;
    
class ResourceManager : public IResourceManager {
public:
    string GetResourcePath() const
    {
        NSString* bundlePath =[[NSBundle mainBundle] resourcePath];
        return [bundlePath UTF8String];
    }
    
    unsigned int largerMinPOT(unsigned int imgSize)
    {
        unsigned int recSize = 1;
        
        while ( recSize < imgSize )//작으면
            recSize *= 2;
        
        //크거나 같은 수를 리턴
        return recSize;
    }
    
    TextureDescription LoadImage(const string& file)//, bool bUseLocalTex)
    {
        TextureDescription description;
       
        NSString* basePath  = [NSString stringWithUTF8String:file.c_str()];
        UIImage* uiImage    = [UIImage imageWithContentsOfFile:basePath];
        UIImage* scaledImg;
        
        //Check file/image
        if(uiImage==NULL)
        {
            description.Size.x  = description.Size.y  = 0;
            return description;
        }
        
 
        description.Size.x = (int)CGImageGetWidth(uiImage.CGImage);
        description.Size.y = (int)CGImageGetHeight(uiImage.CGImage);
        unsigned int recWidth = largerMinPOT((unsigned int)description.Size.x);
        unsigned int recHeight= largerMinPOT((unsigned int)description.Size.y);
        
//#warning 텍스쳐 용량을 팍 줄이는 임시 코드임.
      //  if(recHeight>64)    recHeight=64;
      //  if(recWidth>64)     recWidth=64;
        
        //Check NPOT
        if (recWidth!=description.Size.x || recHeight!=description.Size.y)//NPOT
        {
            
            UIGraphicsBeginImageContext(CGSizeMake(recWidth, recHeight));
            CGContextRef context = UIGraphicsGetCurrentContext();
            CGContextTranslateCTM(context, 0.0, recHeight);
            CGContextScaleCTM(context, 1.0, -1.0);
            
            CGContextDrawImage(context, CGRectMake(0.0, 0.0, recWidth, recHeight), [uiImage CGImage]);
            scaledImg = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
            
            description.Size.x = recWidth;
            description.Size.y = recHeight;
            
            //NSLog(@"NPOT - %@",basePath);
            
        }
        else
            scaledImg = uiImage;
        

        description.BitPerComponent = 8;
        description.Format = TextureFormatRgba;
        
        int bpp = description.BitPerComponent / 2;
        int byteCount = description.Size.x * description.Size.y * bpp;
        unsigned char* data = (unsigned char*) calloc(byteCount, 1);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
        CGContextRef context = CGBitmapContextCreate(data,
                                                     description.Size.x,
                                                     description.Size.y,
                                                     description.BitPerComponent,
                                                     bpp * description.Size.x,
                                                     colorSpace,
                                                     bitmapInfo);
        CGColorSpaceRelease(colorSpace);
        CGRect rect = CGRectMake(0, 0, description.Size.x, description.Size.y);
        CGContextDrawImage(context, rect, scaledImg.CGImage);
        CGContextRelease(context);
        
         m_imageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
        
        
        uiImage = nil;
        scaledImg = nil;
        
        return description;
    }
    
    TextureDescription LoadResourcePngImage(const string& name)
    {
        //Texture info
        TextureDescription  description;

        NSString* basePath  = [[NSString alloc] initWithUTF8String:name.c_str()];
        UIImage* uiImage    = [[UIImage alloc] initWithContentsOfFile:basePath];
        UIImage* scaledImg;
       
        //Check file/image
        if(uiImage==NULL)
        {
            description.Size.x  = description.Size.y  = 0;
            [basePath release];
            return description;
        }
        
        
        //NPOT processing
        description.Size.x = (int)CGImageGetWidth(uiImage.CGImage);
        description.Size.y = (int)CGImageGetHeight(uiImage.CGImage);
        
        unsigned int recWidth = largerMinPOT((unsigned int)description.Size.x);
        unsigned int recHeight= largerMinPOT((unsigned int)description.Size.y);

//#warning 텍스쳐 용량을 팍 줄이는 임시 코드임.
     //   if(recHeight>64)    recHeight=64;
     //   if(recWidth>64)     recWidth=64;
        
        //Check NPOT
        if (recWidth!=description.Size.x || recHeight!=description.Size.y)//NPOT
        {
            
            UIGraphicsBeginImageContext(CGSizeMake(recWidth, recHeight));
            CGContextRef context = UIGraphicsGetCurrentContext();
            CGContextTranslateCTM(context, 0.0, recHeight);
            CGContextScaleCTM(context, 1.0, -1.0);
            
            CGContextDrawImage(context, CGRectMake(0.0, 0.0, recWidth, recHeight), [uiImage CGImage]);
            scaledImg = UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
            
            description.Size.x = recWidth;
            description.Size.y = recHeight;
            
            //NSLog(@"NPOT - %@",basePath);
            
        }
        else
            scaledImg = uiImage;
        
        
        CGImageRef cgImage  = scaledImg.CGImage;
        CFDataRef dataRef   = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        
        if (m_imageData) [m_imageData release];
        m_imageData         = [NSData dataWithData:(NSData*)dataRef];
        
        //Alpha & Color
        bool hasAlpha               = CGImageGetAlphaInfo(cgImage) !=kCGImageAlphaNone;
        CGColorSpaceRef colorSpace  = CGImageGetColorSpace(cgImage);
        
        switch (CGColorSpaceGetModel(colorSpace))
        {
            case kCGColorSpaceModelMonochrome:
            {
                if (hasAlpha)
                {
                    description.Format = TextureFormatGrayAlpha;        //std::cout << "PNG = Mono-Alpha" << std::endl;
                }
                else
                {
                    description.Format = TextureFormatGray;             //std::cout << "PNG = Mono" << std::endl;
                }
            }
            break;

            case kCGColorSpaceModelRGB:
            {
                if (hasAlpha)
                {
                    description.Format = TextureFormatRgba;     //std::cout << "PNG = RGB-Alpha" << std::endl;
                }
                else
                {
                    description.Format = TextureFormatRgb;      //std::cout << "PNG = RGB" << std::endl;
                }
            }
            break;
            default:
                assert(!"Unsupported color space in PNG.");
                break;
        }
        description.BitPerComponent = (int)CGImageGetBitsPerComponent(cgImage);
        
        
        [uiImage release];
        [basePath release];
        
        uiImage = nil;
        basePath = nil;
        scaledImg = nil;

        return description;
    }
    
    void* GetImageData()
    {
        //return (void*) CFDataGetBytePtr(m_imageData);
        return (void*)[m_imageData bytes];
    }
    ivec2 GetImageSize()
    {
        return m_imageSize;
    }
    void UnloadImage()
    {
        m_imageData = 0;
    }
private:
    NSData* m_imageData;
    ivec2 m_imageSize;
};

IResourceManager* CreateResourceManager()
{
    return new ResourceManager();
}
