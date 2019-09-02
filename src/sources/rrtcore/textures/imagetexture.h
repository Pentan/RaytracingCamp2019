#ifndef R1H_IMAGE_TEXTURE_H
#define R1H_IMAGE_TEXTURE_H

#include <string>
#include "texture.h"

namespace r1h {

class ImageTexture : public Texture {
public:
	enum Interpolate {
		kNearest,
		kLinear
	};

	ImageTexture();
	~ImageTexture();

    void init(const Color *src, int w, int h, int ipo=kNearest);
	bool load(const std::string& path, int ipo=kNearest, int flip=0, R1hFPType gamma=2.2, R1hFPType power=1.0);

	virtual Color sample(const Vector3 &p) const;
	
private:
	Color *texels;
	int width;
	int height;
	int interpolate;
};

}
#endif
