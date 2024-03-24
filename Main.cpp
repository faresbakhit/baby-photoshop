//
// FCAI – Structured Programming – 2024 - Assignment 3 - Task 1
//
// File: CS112_A3_T1_20230022_20230240_20230277
// Purpose: Baby photoshop
//
// Authors:
// * Ahmed Shaaban Maghraby Mohammed - S23 - 20230022
// * Ali Ahmed Mohamed Reda - S23 - 20230240
// * Fares Ahmed Bakhit Hussain - S23 - 20230277
//
// [Ali Ahmed Mohamed Reda]: aliahmedreda34@gmail.com
// [Ahmed Shaaban Maghraby Mohammed]: modymansh05@gmail.com
// [Fares Ahmed Bakhit Hussain]: faresa.bakhit@gmail.com
//

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

class Image
{
public:
    unsigned char* raw_image = nullptr;
    int width = 0;
    int height = 0;
    int channels = 3;

    Image() = default;

    Image(int width, int height) : width(width), height(height) {
        raw_image = (unsigned char*)malloc(width * height * this->channels);
    }

    ~Image() {
        if (raw_image != nullptr) {
            stbi_image_free(raw_image);
        }
    }

    void load(const string& filepath) {
        raw_image = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb);
        if (raw_image == nullptr) {
            throw invalid_argument(stbi_failure_reason());
        }
    }

    void save(const string& filepath) {
        const char* extension = strrchr(filepath.c_str(), '.');
        int success;

        if (!extension) {
            throw invalid_argument("missing image file extension");
        }

        if (strcmp(extension, ".png") == 0) {
            success = stbi_write_png(filepath.c_str(), width, height, STBI_rgb, raw_image, 0);
        } else if (strcmp(extension, ".bmp") == 0) {
            success = stbi_write_bmp(filepath.c_str(), width, height, STBI_rgb, raw_image);
        } else if (strcmp(extension, ".tga") == 0) {
            success = stbi_write_tga(filepath.c_str(), width, height, STBI_rgb, raw_image);
        } else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
            success = stbi_write_jpg(filepath.c_str(), width, height, STBI_rgb, raw_image, 90);
        } else {
            throw invalid_argument("unsupported image format");
        }

        if (!success) {
            throw invalid_argument("couldn't save image in path");
        }
    }

    unsigned char& getPixel(int x, int y, int c) {
        if (x > width || x < 0) {
            throw out_of_range("row out of bounds");
        }
        if (y > height || y < 0) {
            throw out_of_range("column out of bounds");
        }
        if (c > channels || c < 0) {
            throw out_of_range("channel out of bounds");
        }
        return raw_image[(y * width + x) * channels + c];
    }

    unsigned char& operator()(int x, int y, int c) {
        return getPixel(x, y, c);
    }
};

void InvertImage(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = 0xFF - image(i, j, k);
            }
        }
    }
}

void RotateImage(Image& image, int rotations) {
    for (int n = 0; n < rotations; n++) {
        Image flipped_image(image.height, image.width);
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < 3; ++k) {
                    flipped_image(image.height - j, i, k) = image(i, j, k);
                }
            }
        }
        swap(image.raw_image, flipped_image.raw_image);
        swap(image.width, image.height);
    }
}

void FrameImageDrawEdge(Image &image, int width, int pos, int color[3]) {
    for (int i = pos; i < image.width - pos; ++i) {
        for (int j = pos; j < pos + width; ++j) {
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = color[k];
                image(i, image.height + j - width - 2 * pos, k) = color[k];
            }
        }
    }
    for (int j = pos; j < image.height - pos; ++j) {
        for (int i = pos; i < pos + width; ++i) {
            for (int k = 0; k < 3; ++k) {
                image(i, j, k) = color[k];
                image(image.width + i - width - 2 * pos, j, k) = color[k];
            }
        }
    }
}

void FrameImage(Image &image, int color[3]) {
    int frame_width = image.height / 64; // px
    int inner_frame_width = image.height / 128; // px
    int white[3] = {255, 255, 255};
    FrameImageDrawEdge(image, frame_width, 0, color);
    FrameImageDrawEdge(image, inner_frame_width, frame_width, white);
    FrameImageDrawEdge(image, inner_frame_width, frame_width + 3 * inner_frame_width, white);
}

int iinteger(istream& in, const char* p) {
    int i;
    while (true) {
        cout << p;
        in >> i;
        if (in.fail()) {
            in.clear();
            in.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "> error: input not an integer." << endl;
        } else {
            in.ignore();
            return i;
        }
    }
}

int irange(istream& in, const char* p, int min, int max) {
    int i;
    while (true) {
        i = iinteger(cin, p);
        if (i < min || i > max) {
            cout << "> error: option " << i << " not in range [" << min << '-' << max << "]." << endl;
        } else {
            return i;
        }
    }
}

void BlackAndWhite(Image& image) {
  for (int i = 0; i < image.width; ++i) {
    for (int j = 0; j < image.height; ++j) {
      int r = image(i, j, 0);
      int g = image(i, j, 1);
      int b = image(i, j, 1);
      int grayScale = (r + g + b) / 3;
      if (grayScale > 126) {
        image(i, j, 0) = 255;
        image(i, j, 1) = 255;
        image(i, j, 2) = 255;
      } else {
        image(i, j, 0) = 0;
        image(i, j, 1) = 0;
        image(i, j, 2) = 0;
      }
    }
  }
}

void FlipHorizontally(Image& image) {
    int width = image.width;
    int middle = width / 2;
    bool isEven = width % 2 == 0;
    int end = isEven ? middle - 2 : middle - 1;
    for (int i = 0; i < end; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int c = 0; c < 3; c++) {
                int temp = image(i, j, c);
                image(i, j, c) = image(width - i - 1, j, c);
                image(width - i - 1, j, c) = temp;
            }
        }
    }
    if (isEven) {
        for (int j = 0; j < image.height; j++) {
            for (int c = 0; c < 3; c++) {
                int temp = image(middle, j, c);
                image(middle, j, c) = image(middle - 1, j, c);
                image(middle - 1, j, c) = temp;
            }
        }
    }
}

void FlipVertical(Image& image) {
    int height = image.height;
    int middle = height / 2;
    bool isEven = height % 2 == 0;
    int end = isEven ? middle - 2 : middle - 1;
    for (int j = 0; j < end; j++) {
        for (int i = 0; i < image.width; ++i) {
            for (int c = 0; c < 3; c++) {
                int temp = image(i, j, c);
                image(i, j, c) = image(i, height - j - 1, c);
                image(i, height - j - 1, c) = temp;
            }
        }
    }
    if (isEven) {
        for (int i = 0; i < image.width; ++i) {
            for (int c = 0; c < 3; c++) {
                int temp = image(i, middle, c);
                image(i, middle, c) = image(i, middle - 1, c);
                image(i, middle - 1, c) = temp;
            }
        }
    }
}

int main() {
    while (true) {
        cout << "> 1. Open new image" << endl
             << "> 2. Exit" << endl;

        int action = irange(cin, ">> ", 1, 2);
        if (action == 2) {
            break;
        }

        Image image;
        while (true) {
            string filepath;
            cout << ">> Enter image path: ";
            getline(cin, filepath);

            try {
                image.load(filepath);
                break;
            } catch(const exception& e) {
                cout << "> error: " << e.what() << endl;
                continue;
            }
        }

        while (true) {
            cout 
                // << "> 1. Grayscale" << endl
                << "> 2. Black and White" << endl
                << "> 3. Invert" << endl
                // << "> 4. Merge Images" << endl
                << "> 5. Flip" << endl
                << "> 6. Rotate" << endl
                // << "> 7. Darken or Lighten" << endl
                // << "> 8. Crop Image" << endl
                << "> 9. Frame" << endl
                // << "> 10. Edges" << endl
                // << "> 11. Resize" << endl
                // << "> 12. Blur" << endl
                << "> 13. Save" << endl;

            int filter = irange(cin, ">> ", 1, 13);
            if (filter == 13) {
                break;
            }

            switch (filter) {
            case 2:
                BlackAndWhite(image);
                break;
            case 3:
                InvertImage(image);
                break;
            case 5: {
                cout << "> 1. Flip Horizontally" << endl
                     << "> 2. Flip Vertically" << endl;
                bool flipHorizontally = irange(cin, ">> ", 1, 2) == 1;
                if (flipHorizontally) {
                    FlipHorizontally(image);
                } else {
                    FlipVertical(image);
                }
                break;
            }
            case 6:
                cout << "> 1. Rotate by 90 degrees" << endl
                     << "> 2. Rotate by 180 degrees" << endl
                     << "> 3. Rotate by 270 degrees" << endl;
                RotateImage(image, irange(cin, ">> ", 1, 3));
                break;
            case 9: {
                int color[3] = {
                    irange(cin, ">> Frame color (R)GB: ", 0, 255),
                    irange(cin, ">> Frame color R(G)B: ", 0, 255),
                    irange(cin, ">> Frame color RG(B): ", 0, 255),
                };
                FrameImage(image, color);
                break;
            }
            }
        }

        while (true) {
            string filepath;
            cout << ">> Enter new image path: ";
            getline(cin, filepath);

            try {
                image.save(filepath);
                break;
            } catch(const exception& e) {
                cout << "> error: " << e.what() << endl;
                continue;
            }
        }
    }
}
