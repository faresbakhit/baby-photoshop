//
// FCAI – Structured Programming – 2024 - Assignment 3 - Task 1
//
// File: CS112_A3_T1_20230240_20230022_20230277
// Purpose: Baby photoshop
//
// Authors:
// * Ali Ahmed Mohamed Reda - S23 - 20230240
// * Ahmed Shaaban Maghraby Mohammed - S23 - 20230022
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
private:
    stbi_uc *raw_image = nullptr;
public:
    int width = 0;
    int height = 0;
    int channels = 3;

    Image() = default;

    Image(int width, int height) : width(width), height(height) {
        raw_image = (stbi_uc*)malloc(width * height * this->channels);
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
            success = stbi_write_jpg(filepath.c_str(), width, height, STBI_rgb, raw_image, 100);
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

void grayscale_conversion(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned  int avg = 0; // Initialize average value

            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k); // Accumulate pixel values
            }

            avg /= 3; // Calculate average

            // Set all channels to the average value
            image(i, j, 0) = avg;
            image(i, j, 1) = avg;
            image(i, j, 2) = avg;
        }
    }
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
            cout << "> 1. Grayscale conversion" << endl
                 << "> 2. Save image" << endl;

            int filter = irange(cin, ">> ", 1, 2);
            if (filter == 2) {
                break;
            }

            switch (filter) {
            case 1:
                grayscale_conversion(image);
                break;
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
