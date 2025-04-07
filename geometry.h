class myPoint3D {
public:
    double X, Y, Z;

    myPoint3D(double x = 0, double y = 0, double z = 0)
        : X(x), Y(y), Z(z) {}
};

class myVector3D {
public:
    double dX, dY, dZ;

    myVector3D(double dx = 0, double dy = 0, double dz = 0)
        : dX(dx), dY(dy), dZ(dz) {}
};
