#include "HartreeFock.hh" 

using namespace arma;

HartreeFock::HartreeFock(){
  for (int i =0; i < NUM_STATES; i++){
    for (int j = 0; j < NUM_STATES; j++){
      for (int k = 0; k < NUM_STATES; k++){
        for (int l = 0; l < NUM_STATES; l++){
          matrix_elements[i][j][k][l] = 0.0;
        }
      }
    }
  }

  hamiltonian =   zeros(NUM_STATES,NUM_STATES);
  density_matrix= zeros(NUM_STATES,NUM_STATES);
  eigenvectors =  eye(NUM_STATES,NUM_STATES);
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
  mat harmonic_oscillator_energies = zeros(NUM_STATES,NUM_STATES);
  std::cout << "Reading single particle states from file: " << in_sp_file_name << std::endl;
  ReadSingleParticleStates(in_sp_file_name);
  if (single_particle_states.size() != NUM_STATES){

    std::cout << "ERROR: Failed to get correct number of single particle states!" << std::endl;
    std::cout << "Expect: " << NUM_STATES << "\tFound: " << single_particle_states.size() << std::endl;
    return;
  }
  for (unsigned int i = 0; i < single_particle_states.size(); i++){
    harmonic_oscillator_energies(i,i) = single_particle_states.at(i).GetEnergy();
 //   prev_energies(i) = harmonic_oscillator_energies(i,i); 
  }
  std::cout << "Reading two body matrix elements from file: " << in_mat_file_name << std::endl;
  ReadMatrixElements(in_mat_file_name);


  double single_particle_potential = 0;
  std::cout << "Starting iterations..."<< std::endl;
  while(iteration < MAX_ITERATIONS){
    std::cout << "Completed iteration: " << iteration << std::endl; 
    for (int alpha = 0; alpha < NUM_STATES; alpha++){
      for (int beta = 0; beta < NUM_STATES; beta++){
        single_particle_potential = 0;
        for (int gamma = 0; gamma < NUM_STATES; gamma++){
          for (int delta = 0; delta < NUM_STATES; delta++){
            single_particle_potential += density_matrix(gamma,delta)*matrix_elements[alpha][beta][gamma][delta];
          }//loop over delta
        }//loop over gamma
        if (single_particle_potential != 0){
          std::cout << "Single particle potential = " << single_particle_potential << std::endl;
        }
        //Hamiltonian is Hermitian
        hamiltonian(alpha,beta) = harmonic_oscillator_energies(alpha,beta) + single_particle_potential;
        hamiltonian(beta,alpha) = harmonic_oscillator_energies(alpha,beta) + single_particle_potential;
      }//loop over beta
    }//loop over alpha
  
//  std::cout << "Eigenvectors Before" << std::endl;
//  std::cout << eigenvectors << std::endl;
    eig_sym(energies,eigenvectors,hamiltonian);
//  std::cout << "Eigenvectors After" << std::endl;
//  std::cout << eigenvectors << std::endl;

    if (iteration > 0 && IsConverged()){
      std::cout << "Converged after " << iteration<< "!" << std::endl;
      break;
    }
    FillDensityMatrix();
    prev_energies = energies;
    iteration++;
  }//while iteration < MAX_ITERATIONS and not converged
  SaveToFile(out_file_name, harmonic_oscillator_energies);
}


bool HartreeFock::IsConverged() const {
  const double THRESHOLD = pow(10,-8);
  vec diff = energies - prev_energies;
  std::cout << "DIFF" << std::endl;
  std::cout << diff << std::endl;
  if (abs(diff.max()) < THRESHOLD){
    return true;
  }
  else {
    return false;
  }
}

void HartreeFock::SaveToFile(std::string const &file_name, mat &harmonic_oscillator_energies) const{
  std::ofstream out_file;
  out_file.open(file_name.c_str());
  if (!out_file.is_open()){
    std::cout << "ERROR: Failed to open output file!" << std::endl;
  }

  out_file << "HO Energies\tCalculated Energies" << std::endl;
  for (int i = 0; i < NUM_STATES; i++){
    out_file << harmonic_oscillator_energies(i,i) <<"\t"<<energies(i) << std::endl; 
  }
}

void HartreeFock::ReadMatrixElements(std::string const &file_name){
  std::ifstream input_file;
  input_file.open(file_name.c_str());
  if (!input_file.is_open()){
    std::cout << "ERROR: Failed to open matrix elements file" << std::endl;
    return;
  }

  int a = -1;
  int b = -1;
  int c = -1; 
  int d = -1;
  double matrix_element =-1;
  std::string line;
  std::cout << "Starting to parse matrix elements..." << std::endl;
  while(std::getline(input_file,line)){
    sscanf(line.c_str(), "%d %d %d %d %lf", &a,&b,&c,&d,&matrix_element); 
    //minus one because want to be referenced from 0
    matrix_elements[a-1][b-1][c-1][d-1] = matrix_element;   
  }
  std::cout << "Finished parsing matrix elements" << std::endl;
  return;
}

void HartreeFock::ReadSingleParticleStates(std::string const &file_name){
  std::ifstream input_file;
  input_file.open(file_name.c_str());
  if (!input_file.is_open()){
    std::cout << "Failed to open matrix elements file" << std::endl;
    return;
  }

  int state_index =-1;
  int n = -1;
  int l = -1; 
  int j2 = -1;
  int mj2 = -1;
  int tz2 = -1;
  std::string line;
  while(std::getline(input_file,line)){
    sscanf(line.c_str(), "Orbit number: %d %d %d %d %d %d", &state_index,&n,&l,&j2,&mj2,&tz2); 
    State state(state_index,n,l,j2,mj2,tz2);
    single_particle_states.push_back(state);
  }
  return;
}

void HartreeFock::FillDensityMatrix(){
  density_matrix = zeros(NUM_STATES,NUM_STATES);
  //  density_matrix = eigenvectors*eigenvectors
  for (int gamma = 0; gamma < NUM_STATES; gamma++){
    for (int delta = 0; delta < NUM_STATES; delta++){
      for(int state = 0; state < NUM_STATES; state++){
        density_matrix(gamma,delta) += eigenvectors(state,delta)*eigenvectors(state,gamma);
      }
    }
  }
}


