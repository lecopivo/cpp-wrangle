#include <iostream>

//#include "simplex.h"
#include "fem.h"

#include <Eigen/Geometry>

using namespace houfem;

int main() {

  std::cout << "Hello simplex!" << std::endl;

  // {
  //   Eigen::Matrix<double, 2, 3> triangle_points;
  //   triangle_points << -1, 2, 1, -1, 1, 2;

  //   std::cout << "Triangle:\n" << triangle_points << "\n\n";

  //   Simplex<2> triangle{triangle_points};

  //   std::cout << "from unit simplex:\nA:\n"
  //             << triangle.from_unit_simplex.A << "\nb:\n"
  //             << triangle.from_unit_simplex.b << "\n\n";

  //   std::cout << "to unit simplex:\nA:\n"
  //             << triangle.to_unit_simplex.A << "\nb:\n"
  //             << triangle.to_unit_simplex.b << "\n";

  //   auto phi = std::array{triangle.barycentric_coordinate(0),
  //                         triangle.barycentric_coordinate(1),
  //                         triangle.barycentric_coordinate(2)};

  //   auto integrator1 = triangle.integrator(1);
  //   auto integrator0 = triangle.integrator(0);

  //   Eigen::Matrix<double, 3, 3> M;
  //   Eigen::Matrix<double, 3, 3> S;
  //   for (int i = 0; i < 3; i++) {
  //     for (int j = 0; j < 3; j++) {
  //       M(i, j) = integrator1([&](Eigen::Matrix<double, 2, 1> x) {
  //         return phi[i](x) * phi[j](x);
  //       });

  //       S(i, j) = integrator0([&](Eigen::Matrix<double, 2, 1> x) {
  //         return phi[i].derivative()(x).dot(phi[j].derivative()(x));
  //       });
  //     }
  //   }

  //   std::cout << std::endl;
  //   std::cout << "Mass Matrix:\n" << M << "\n\n";
  //   std::cout << "Mass Matrix Eigenvalues:\n"
  //             << M.eigenvalues().transpose() << "\n\n";
  //   std::cout << "Stiffness Matrix:\n" << S << "\n\n";
  //   std::cout << "Stiffness Matrix Eigenvalues:\n"
  //             << S.eigenvalues().transpose() << "\n\n";
  // }

  // {
  //   Eigen::Vector3d axis = Eigen::Vector3d::Random().normalized();
  //   Eigen::AngleAxis<double>    R(M_PI/6, axis);
  //   Eigen::Matrix<double, 3, 3> triangle_points;
  //   triangle_points << -1, 2, 1, -1, 1, 2, 0, 0, 0;
  //   triangle_points = R*triangle_points;

  //   std::cout << "Embedded Triangle:\n" << triangle_points << "\n\n";

  //   EmbeddedSimplex<2, 3> triangle{triangle_points};

  //   std::cout << "global_to_local:\nA:\n"
  //             << triangle.global_to_local.A << "\nb:\n"
  //             << triangle.global_to_local.b << "\n\n";

  //   std::cout << "local_to_global:\nA:\n"
  //             << triangle.local_to_global.A << "\nb:\n"
  //             << triangle.local_to_global.b << "\n";

  //   std::cout << "from unit simplex:\nA:\n"
  //             << triangle.local_simplex.from_unit_simplex.A << "\nb:\n"
  //             << triangle.local_simplex.from_unit_simplex.b << "\n\n";

  //   std::cout << "to unit simplex:\nA:\n"
  //             << triangle.local_simplex.to_unit_simplex.A << "\nb:\n"
  //             << triangle.local_simplex.to_unit_simplex.b << "\n";

  //   auto phi = std::array{triangle.local_simplex.barycentric_coordinate(0),
  //                         triangle.local_simplex.barycentric_coordinate(1),
  //                         triangle.local_simplex.barycentric_coordinate(2)};

  //   auto integrator1 = triangle.local_simplex.integrator(1);
  //   auto integrator0 = triangle.local_simplex.integrator(0);

  //   Eigen::Matrix<double, 3, 3> M;
  //   Eigen::Matrix<double, 3, 3> S;
  //   for (int i = 0; i < 3; i++) {
  //     for (int j = 0; j < 3; j++) {
  //       M(i, j) = integrator1([&](Eigen::Matrix<double, 2, 1> x) {
  //         return phi[i](x) * phi[j](x);
  //       });

  //       S(i, j) = integrator0([&](Eigen::Matrix<double, 2, 1> x) {
  //         return phi[i].derivative()(x).dot(phi[j].derivative()(x));
  //       });
  //     }
  //   }

  //   std::cout << std::endl;
  //   std::cout << "Mass Matrix:\n" << M << "\n\n";
  //   std::cout << "Mass Matrix Eigenvalues:\n"
  //             << M.eigenvalues().transpose() << "\n\n";
  //   std::cout << "Stiffness Matrix:\n" << S << "\n\n";
  //   std::cout << "Stiffness Matrix Eigenvalues:\n"
  //             << S.eigenvalues().transpose() << "\n\n";
  // }

  return 0;
}
