#include "Visao.h"

Mat imagemHSV;
int pontos[3];
bool clique = false;

Visao::Visao(){    
    cam = VideoCapture(0);
    variacao = 0.2;
    variacaoH = 4;    
    calibrar = false;
}    

int Visao::tempo(){
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

void Visao::tratarImagemComCor(Mat imagem, Cor cor){
    inRange(imagemHSV, Scalar(cor.corH-variacaoH, cor.corS*(1-variacao), cor.corV*(1-variacao)),
     Scalar(cor.corH+variacaoH, cor.corS*(1+variacao), cor.corV*(1+variacao)), imagemTratada);
}

void Visao::corPixel(int evt, int x, int y, int flags, void* param){    
    if (evt == CV_EVENT_LBUTTONDOWN){
        Vec3b p = imagemHSV.at<Vec3b>(y,x);
        printf("(%d, %d, %d)\n", p[0], p[1], p[2]);
        pontos[0] = (int)p[0];
        pontos[1] = (int)p[1];
        pontos[2] = (int)p[2];  
        clique = true;
    }   
}

Cor Visao::gravaCor(){        
    Cor cor = Cor();

    while (true){ 
        clock_t start=tempo();  

        cam >> imagem;

        cvtColor(imagem, imagemHSV, COLOR_BGR2HSV); 

        imshow("Imagem Normal", imagem); 
        moveWindow("Imagem Normal", 0, 0);  

        inRange(imagemHSV, Scalar(pontos[0]-variacaoH, pontos[1]*(1-variacao), pontos[2]*(1-variacao)),
         Scalar(pontos[0]+variacaoH, pontos[1]*(1+variacao), pontos[2]*(1+variacao)), imagemTratada); 
        
        //medianBlur(imagemTratada, imagemTratada, 3);     

        imshow("Calibragem", imagemTratada); 
        moveWindow("Calibragem", 640, 0);

        setMouseCallback("Imagem Normal", corPixel, 0);

        char key =  waitKey(1);    
        if (key == 27){
            for (int i=0; i<10; i++){
                destroyAllWindows();
                waitKey(1);  
            }
            break; 
        }
        if (clique == true){
            cor.corH = pontos[0];
            cor.corS = pontos[1];
            cor.corV = pontos[2];

            if (key == 32){ 
                for (int i=0; i<10; i++){
                    destroyAllWindows();
                    waitKey(1);  
                }
                cout << "Cor Calibrada" << endl;
                clique = false;
                pontos[0] = 0; pontos[1] = 0; pontos[2] = 0;  
                break; 
            }            
        }        
    }
    return cor;
}
