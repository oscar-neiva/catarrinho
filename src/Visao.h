#ifndef VISAO_H_
#define VISAO_H_

#include <iostream>
#include <stdio.h>
#include "unistd.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Cor.h"

using namespace cv;
using namespace std;


class Visao{
private:
	VideoCapture cam;  
    Mat imagem;
    Mat imagemTratada;  
    int variacaoH;
    float variacao;
    bool calibrar;
public:
	Visao();
    int tempo();
    static void corPixel(int evt, int x, int y, int flags, void* param);
    void tratarImagemComCor(Mat imagem, Cor cor);    
    void calibragem(); 
    Cor gravaCor();
};
#endif