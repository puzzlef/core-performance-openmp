#include <algorithm>
#include <memory>
#include <tuple>
#include <vector>
#include <random>
#include <chrono>
#include <cstdio>
#include <omp.h>
#include <sched.h>

using namespace std;




#pragma region CONFIGURATION
#ifndef TYPE
/** Type of edge weights. */
#define TYPE float
#endif
#ifndef MAX_THREADS
/** Maximum number of threads to use. */
#define MAX_THREADS 64
#endif
#ifndef REPEAT_METHOD
/** Number of times to repeat each method. */
#define REPEAT_METHOD 5
#endif
#pragma endregion




#pragma region POPULATE RANDOM
/**
 * Populates a vector with random values.
 * @param a vector to populate
 */
template <class T>
inline void populateRandomValues(vector<T>& a) {
  size_t N = a.size();
  random_device dev;
  default_random_engine rnd(dev());
  uniform_real_distribution<T> dis(0, 1);
  for (size_t i=0; i<min(N, 1024); ++i)
    a[i] = dis(rnd);
  for (size_t i=1024; i<N; ++i)
    a[i] = a[i % 1024];
}
#pragma endregion




#pragma region COMPUTE SUM
/**
 * Result of a tracked sum computation.
 */
template <class T>
struct ComputeSumResult {
  /** Sum of the vector. */
  T   sum;
  /** Core ID on which the sum was computed. */
  int coreId;
  /** Time it took to compute the sum, in milliseconds. */
  float time;
};


/**
 * Computes the sum of a vector, using OpenMP.
 * @param x vector to compute the sum of
 * @returns sum of the vector
 */
template <class T>
inline T computeSumOmp(const vector<T>& x) {
  size_t N = x.size();
  T sum = T();
  #pragma omp parallel for schedule(static, 2048) reduction(+:sum)
  for (size_t i=0; i<N; ++i)
    sum += x[i];
  return sum;
}


template <class T>
inline auto computeSumTrackedOmp(const vector<T>& x) {
  size_t N = x.size();
  // Allocate memory for per-thread sums.
  vector<unique_ptr<ComputeSumResult<T>>> a(N);
  for (size_t i=0; i<N; ++i)
    a[i] = make_unique<ComputeSumResult<T>>();
  // Compute per-thread sums.
  #pragma omp parallel
  {
    int t = omp_get_thread_num();
    a[t]->sum    = T();
    a[t]->coreId = sched_getcpu();
    auto t0      = chrono::high_resolution_clock::now();
    // Compute sum.
    *sum[t]    = T();
    for (size_t i=t; i<N; ++i)
      *sum[t] += x[i];
    // Compute time.
    auto t1    = chrono::high_resolution_clock::now();
    a[t]->time = chrono::duration_cast<chrono::microseconds>(t1 - t0).count() / 1000.0f;
  }
  return a;
}
#pragma endregion




#pragma region PERFORM EXPERIMENT
/**
 * Run the experiment.
 * @param x vector to compute the sum of
 */
template <class T>
inline void runExperiment(const vector<T>& x) {
  int repeat = REPEAT_METHOD;
  int    T = omp_get_max_threads();
  size_t N = x.size();
  T sum = computeSumOmp(x);
  for (int l=0; l < repeat; ++l) {
    auto a = computeSumTrackedOmp(x);
    for (int t=0; t<T; ++t) {
      double flops = 1000.0 * N / a[t]->time;
      printf("{run=%02d, thread=%03d, core=%03d, time=%09.1fms, flops=%.2e}\n", l, t, a[t]->coreId, a[t]->time, flops);
    }
  }
}


/**
 * Main function.
 * @param argc argument count
 * @param argv argument values
 * @returns 0 on success, error code otherwise
 */
int main(int argc, char **argv) {
  using  T = TYPE;
  size_t N = argc > 1? atoi(argv[1]) : 1000000;
  omp_set_num_threads(MAX_THREADS);
  LOG("OMP_NUM_THREADS=%d\n", MAX_THREADS);
  vector<T> x(N);
  populateRandomValues(x);
  runExperiment(x);
}
#pragma endregion
