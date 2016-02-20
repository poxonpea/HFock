#include "HartreeFock.hh" 

using namespace arma;

HartreeFock::HartreeFock(){
  for (int i =0; i < NUM_STATES; i++){
    for (int j = 0; j < NUM_STATES; j++){
      for (int k = 0; k < NUM_STATES; k++){
        for (int l = 0; l < MUM_STATES; l++){
          matrix_elements[i][j][k][l] = 0.0;
        }
      }
    }
  }

  hamiltonian =   zeros(NUM_STATES,NUM_STATES);
  density_matrix= zeros(NUM_STATES,NUM_STATES);
  eigenvectors =  zeros(NUM_STATES,NUM_STATES);
  energies = zeros(NUM_STATES,1);
  prev_energies = zeros(NUM_STATES,1);
  for (int i =0; i < 16; i++){
      density_matrix(i,i) = 1;
  }
}

HartreeFock::~HartreeFock(){
}

void HartreeFock::Run(std::string const &in_mat_file_name, std::string const &in_sp_file_name,
                      std::string const &out_file_name){
  int iteration = 0;
  arma::matrix harmonic_oscillator_energies = zeros(NUM_STATES,NUM_STATES);
  
  ReadSingleParticleStates(in_sp_file_name);
  ReadMatrixElements(in_mat_file_name);

  for (int i = 0; i < NUM_STATES; i++){
    harmonic_oscillator_energies(i,i) = ;
  }



  double single_particle_potential;
  while(iterations < MAX_ITERATIONS){
    std::cout << "Completed iteration: " << iteration << "\xd"; 
    for (int alpha = 0; alpha < NUM_STATES; alpha++){
      for (int beta = 0; beta < NUM_STATES; beta++){
        single_particle_potential = 0;
        for (int gamma = 0; gamma < NUM_STATES; gamma++){
          for (int delta = 0; delta < NUM_STATES; delta++){
            single_particle_potential += density_matrix(gamma,delta) * matrix_elements(alpha,beta,gamma,delta);
          }//loop over delta
        }//loop over gamma
        //Hamiltonian is Hermitian
        hamiltonian(alpha,beta) = harmonic_oscillator_energies(alpha,beta) + single_particle_potential;
        hamiltonian(beta,alpha) = harmonic_oscillator_energies(alpha,beta) + single_particle_potential;
      }//loop over beta
    }//loop over alpha
  
    FillDensityMatrix();

    if (iteration > 0 && IsConverged()){
        break;
    }
    prev_energies = energies;
  }//while iterations < MAX_ITERATIONS and not converged
}


bool HartreeFock::IsConverged(){
}

void HartreeFock::SaveToFile(std::string const &file_name){
}

void HartreeFock::ReadMatrixElements(std::string const &file_name){
  ifstream input_file;
  input_file.open(file_name.c_str());
  if (!input_file.is_open()){
    std::cout << "Failed to open matrix elements file" << std::endl;
    return;
  }

  int a;
  int b;
  int c; 
  int d;
  double matrix_element;
  std::string line;
  while(std::getline(input_file,line)){
    sscanf(line.c_str(), "%d %d %d %d %f", a,b,c,d,matrix_element); 
    matrix_elements[a][b][c][d] = matrix_element;   
  }
       
  return;
}

void HartreeFock::ReadSingleParticleStates(std::string const &file_name){
  ifstream input_file;
  input_file.open(file_name.c_str());
  if (!input_file.is_open()){
    std::cout << "Failed to open matrix elements file" << std::endl;
    return;
  }

  int state_index;
  int n;
  int l; 
  int j2;
  int mj2;
  int tz2;
  std::string line;
  while(std::getline(input_file,line)){
    sscanf(line.c_str(), "Orbit number: %d %d %d %d %d %d", state_index,n,l,j2,mj2,tz2); 
    State state(state_index,n,l,j2,mj2,tz2);
    single_particle_states.push_back(state);
  }
       
  return;
}

void HartreeFock::FillDensityMatrix(){
}


