#include <iostream>
//#Eigen
#include <Eigen/Core>
#include <Eigen/Dense>

//#Sophus
#include <sophus/se3.h>

#define THRESHOLD 1e-8;
#define RADIUS 1000;
#define FOCUS 100;

using namespace std;

void test();

int main(int argc, char *argv[])
{
    test();
    return 0;
}

Eigen::Vector3d homogeneous(Eigen::Vector3d v, double f){
    Eigen::Vector3d dst;
    dst[0] = -1*v[0]/v[2]*f;
    dst[1] = -1*v[1]/v[2]*f;
    dst[2] = -1*f;
    return dst;
}

bool isGood(Eigen::Matrix<double, 3,1> x, double threshold){
    bool flag = true;
    for(int i=0; i<3; i++){
        if(abs(x(i,0)) > threshold) flag = false;
    }
    return flag;
}


Eigen::MatrixXd pinv(Eigen::MatrixXd A)
{
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV);//M=USV*
    double  pinvtoler = 1.e-8;
    int row = A.rows();
    int col = A.cols();
    int k = min(row,col);
    Eigen::MatrixXd X = Eigen::MatrixXd::Zero(col,row);
    Eigen::MatrixXd singularValues_inv = svd.singularValues();
    Eigen::MatrixXd singularValues_inv_mat = Eigen::MatrixXd::Zero(col, row);
    for (long i = 0; i<k; ++i) {
        if (singularValues_inv(i) > pinvtoler)
            singularValues_inv(i) = 1.0 / singularValues_inv(i);
        else singularValues_inv(i) = 0;
    }
    for (long i = 0; i < k; ++i)
    {
        singularValues_inv_mat(i, i) = singularValues_inv(i);
    }
    X=(svd.matrixV())*(singularValues_inv_mat)*(svd.matrixU().transpose());
    return X;
}


void test(){

    double threshold = THRESHOLD;
    double radius = RADIUS;
    double focus = FOCUS;

    double a1 = sqrt(2)/2.0;
    double a2 = sqrt(2)/2.0;
    double a3 = sqrt(2)/2.0;

    double m = sqrt(3)*radius/3;
    Eigen::Vector3d Pw1(m,m,-1*m);
    Eigen::Vector3d Pw2(-1*m,m,-1*m);
    Eigen::Vector3d Pw3(m,-1*m,-1*m);


    Eigen::Matrix3d R;

//    R << 1,0,0,0,1,0,0,0,1;
    R << 1,0,0,
            0,0.5,-0.866,
            0,0.866,0.5;


    Eigen::Matrix<double, 3,3> B;
    Eigen::Matrix<double, 3,1> l;
    Eigen::Matrix<double, 3,1> x;
    x << 1,1,1;

    int step = 0;
    bool isGoodResult = true;
    int stepnum = 100;

    while(!isGood(x,threshold) && step < stepnum){

        Eigen::Vector3d Qs1, Qs2, Qs3;
        Qs1 = homogeneous(R*Pw1,focus);
        Qs2 = homogeneous(R*Pw2,focus);
        Qs3 = homogeneous(R*Pw3,focus);

        double e1, e2, e3;
        e1 = (Qs1.transpose()*Qs1)(0,0) - (a1*a1+focus*focus);
        e2 = (Qs2.transpose()*Qs2)(0,0) - (a2*a2+focus*focus);
        e3 = (Qs3.transpose()*Qs3)(0,0) - (a3*a3+focus*focus);


        Eigen::Matrix<double, 1,3> de1_dQs1, de2_dQs2, de3_dQs3;
        de1_dQs1 = 2*Qs1.transpose();
        de2_dQs2 = 2*Qs2.transpose();
        de3_dQs3 = 2*Qs3.transpose();


        Eigen::Matrix<double, 3,3> dQs1_dfai, dQs2_dfai, dQs3_dfai;
        dQs1_dfai = -1*Sophus::SO3::hat(R*Pw1);
        dQs2_dfai = -1*Sophus::SO3::hat(R*Pw2);
        dQs3_dfai = -1*Sophus::SO3::hat(R*Pw3);


        Eigen::Matrix<double, 1,3> de1_dfai, de2_dfai, de3_dfai;
        de1_dfai = de1_dQs1*dQs1_dfai;
        de2_dfai = de2_dQs2*dQs2_dfai;
        de3_dfai = de3_dQs3*dQs3_dfai;
//        de1_dfai << 1,1,1;


        B << de1_dfai, de2_dfai, de3_dfai;

        l << -1*e1, -1*e2, -1*e3;

        Eigen::Matrix<double,3,3> N;
        Eigen::Matrix<double,3,1> U;
        N=B.transpose()*B;
        U =B.transpose()*l;

//        x = N.inverse()*U;
        x = pinv(N)*U;

        Eigen::Vector3d deltafai;
        for(int k=0;k<3;k++){
            deltafai[k] = x(k,0);
        }

        R=Sophus::SO3::exp(deltafai).matrix()*R;
        step++;

        if(step==stepnum) {
            isGoodResult =false;
        }
    }


    cout << "done!" << endl;
    if(isGoodResult){
        cout << "Good result!" << endl;
    }
    else{
        cout << "Bad result!" << endl;
    }

}
