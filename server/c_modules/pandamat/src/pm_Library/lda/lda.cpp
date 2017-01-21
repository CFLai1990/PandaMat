#include "lda.h"
#include <stdio.h>
#include <assert.h>
#include <iostream>

LDAParameter::LDAParameter(int v_num_docs, int v_num_topics, int v_num_iters,
  float v_alpha, float v_beta){
  num_docs = v_num_docs;
  num_topics = v_num_topics;
  num_iters = v_num_iters;
  alpha = v_alpha;
  beta = v_beta;
}

int MyLDA::init(int model_status, float alpha, float beta, int n_topics, int n_iters, int savestep, int twords, string dir, string dfile, string model_name)
{
  int K = n_topics;
  int niters = n_iters;
  int withrawdata = 0;

  if (model_status == MODEL_STATUS_EST) {
    if (dfile == "") {
      printf("Please specify the input data file for model estimation!\n");
      return 1;
    }

    this->model_status = model_status;

    if (K > 0) {
      this->K = K;
    }

    if (alpha >= 0.0) {
      this->alpha = alpha;
    } else {
      // default value for alpha
      this->alpha = 50.0 / this->K;
    }

    if (beta >= 0.0) {
      this->beta = beta;
    }

    if (niters > 0) {
      this->niters = niters;
    }

    if (savestep > 0) {
      this->savestep = savestep;
    }

    if (twords > 0) {
      this->twords = twords;
    }

    this->dfile = dfile;

    string::size_type idx = dfile.find_last_of("/");
    if (idx == string::npos) {
      this->dir = "./";
    } else {
      this->dir = dfile.substr(0, idx + 1);
      this->dfile = dfile.substr(idx + 1, dfile.size() - this->dir.size());
      // printf("dir = %s\n", this->dir.c_str());
      // printf("dfile = %s\n", this->dfile.c_str());
    }
  }

  if (model_status == MODEL_STATUS_ESTC) {
    if (dir == "") {
      printf("Please specify model directory!\n");
      return 1;
    }

    if (model_name == "") {
      printf("Please specify model name upon that you want to continue estimating!\n");
      return 1;
    }	

    this->model_status = model_status;

    if (dir[dir.size() - 1] != '/') {
      dir += "/";
    }
    this->dir = dir;

    this->model_name = model_name;

    if (niters > 0) {
      this->niters = niters;
    }

    if (savestep > 0) {
      this->savestep = savestep;
    }

    if (twords > 0) {
      this->twords = twords;
    }

    // read <model>.others file to assign values for ntopics, alpha, beta, etc.
    //    if (read_and_parse(this->dir + this->model_name + this->others_suffix, this)) {
    //      return 1;
    //    }	
  } 

  if (model_status == MODEL_STATUS_INF) {
    if (dir == "") {
      printf("Please specify model directory please!\n");
      return 1;
    }

    if (model_name == "") {
      printf("Please specify model name for inference!\n");
      return 1;
    }	

    if (dfile == "") {
      printf("Please specify the new data file for inference!\n");
      return 1;
    }

    this->model_status = model_status;

    if (dir[dir.size() - 1] != '/') {
      dir += "/";
    }
    this->dir = dir;

    this->model_name = model_name;

    this->dfile = dfile;

    if (niters > 0) {
      this->niters = niters;
    } else {
      // default number of Gibbs sampling iterations for doing inference
      this->niters = 20;
    }

    if (twords > 0) {
      this->twords = twords;
    }

    if (withrawdata > 0) {
      this->withrawstrs = withrawdata;
    }

    // read <model>.others file to assign values for ntopics, alpha, beta, etc.
    //    if (read_and_parse(this->dir + this->model_name + this->others_suffix, this)) {
    //      return 1;
    //    }
  }

  if (model_status == MODEL_STATUS_UNKNOWN) {
    printf("Please specify the task you would like to perform (-est/-estc/-inf)!\n");
    return 1;
  }

  if (model_status == MODEL_STATUS_EST) {
    // estimating the model from scratch
    if (init_est()) {
      return 1;
    }

  } else if (model_status == MODEL_STATUS_ESTC) {
    // estimating the model from a previously estimated one
    if (init_estc()) {
      return 1;
    }

  } else if (model_status == MODEL_STATUS_INF) {
    // do inference
    if (init_inf()) {
      return 1;
    }
  }

  return 0;
}

void MyLDA::getParameters(LDAParameter v_parameters){
  _n_topics = v_parameters.num_topics;
  _n_docs = v_parameters.num_docs;
  _n_iters = v_parameters.num_iters;
  p_alpha = v_parameters.alpha;
  p_beta = v_parameters.beta;
  parameterReady = true;
}

bool MyLDA::run_model(string v_dir, string v_filename)
{
  const bool t_console_output = false;
  if(!parameterReady){
    return false;
  }else{
    // save_words(TRNDATA_FILENAME.c_str(), _n_docs, _words);
    model_file = v_dir + model_final_name;
    trndata_file = v_dir + v_filename;
    wordmap_file = v_dir + "wordmap.txt";
    words_file = v_dir + "words.dat";
    others_file = model_file + ".others";
    theta_file = model_file + ".theta";
    phi_file = model_file + ".phi";
    tassign_file = model_file + ".tassign";

    init(MODEL_STATUS_EST, p_alpha, p_beta, _n_topics, _n_iters, 100, 20, v_dir, trndata_file, model_file);
    #ifndef LDA_SILENT
    fprintf(stderr, "[MyLDA::run_model] Start estimating model\n");
    #endif
    estimate(t_console_output);
    #ifndef LDA_SILENT
    fprintf(stderr, "[MyLDA::run_model] Finish estimating model\n");
    #endif

    // load_wordmap(wordmap_file.c_str(), _n_words, _wordmap, _reverse_wordmap);
    // load_theta(theta_file.c_str(), _n_docs, _n_topics, _theta);
    // load_phi(phi_file.c_str(), _n_topics, _n_words, _phi);
    // load_tassign(tassign_file.c_str(), _n_docs, _tassign, _n_words);   
    return true; 
  }
}

void MyLDA::save_model()
{
  save_words(words_file.c_str(), _n_docs, _words);
}

void MyLDA::load_model()
{
  float alpha, beta;
  load_others(others_file.c_str(), alpha, beta, _n_topics, _n_docs, _n_words);
  load_words(words_file.c_str(), _n_docs, _words);
  fprintf(stderr, "n_docs = %d, n_topics = %d, n_words = %d\n", _n_docs, _n_topics, _n_words);
  load_wordmap(wordmap_file.c_str(), _n_words, _wordmap, _reverse_wordmap);
  load_theta(theta_file.c_str(), _n_docs, _n_topics, _theta);
  load_phi(phi_file.c_str(), _n_topics, _n_words, _phi);
  load_tassign(tassign_file.c_str(), _n_docs, _tassign, _n_words);
}

void MyLDA::save_words(const char *filename, int n_docs, const vector<vector<int> > &words)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::save words] Start saving words\n");
    #endif
  FILE *fp = fopen(filename, "w");
  fprintf(fp, "%d\n", n_docs);
  for (int k = 0; k < n_docs; ++k)
  {
    fprintf(fp, "%d\n", (int)words[k].size());
    for (unsigned int i = 0; i < words[k].size(); ++i)
      fprintf(fp, "%d ", words[k][i]);
    fprintf(fp, "\n");
  }
  fclose(fp);
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::save words] Finish saving words\n");
    #endif
}

void MyLDA::load_others(const char *filename, float &alpha, float &beta, int &n_topics, int &n_docs, unsigned int &n_words)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load others] Start loading others\n");
    #endif
  FILE *fp = fopen(filename, "r");
  fscanf(fp, "alpha=%f\n", &alpha);
  fscanf(fp, "beta=%f\n", &beta);
  fscanf(fp, "ntopics=%d\n", &n_topics);
  fscanf(fp, "ndocs=%d\n", &n_docs);
  fscanf(fp, "nwords==%d\n", &n_words);
  fclose(fp);
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load others] Finish loading others\n");
    #endif
}

void MyLDA::load_words(const char *filename, int n_docs, vector<vector<int> > &words)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load words] Start loading words\n");
    #endif
  FILE *fp = fopen(filename, "r");
  fscanf(fp, "%d", &n_docs);
  words.resize(n_docs);
  for (int k = 0; k < n_docs; ++k)
  {
    int num;
    fscanf(fp, "%d\n", &num);
    words[k].resize(num);
    for (int i = 0; i < num; ++i)
      fscanf(fp, "%d ", &words[k][i]);
  }
  fclose(fp);
    #ifndef LDA_SILENT
  fscanf(stderr, "[MyLDA::load words] Finish loading words\n");
    #endif
}

void MyLDA::load_theta(const char *filename, int n_docs, int n_topics, vector<float> &theta)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_theta] Start reading theta\n");
    #endif

  FILE *fp = fopen(filename, "r");
  theta.resize(n_docs * n_topics);
  for (int k = 0; k < n_docs; ++k)
  {
    for (int i = 0; i < n_topics; ++i)
    {
      float v;
      fscanf(fp, "%f", &v);
      theta[k * n_topics + i] = v;
    }
  }
  fclose(fp);

    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_theta] Finish reading theta\n");
    #endif
}

void MyLDA::load_phi(const char *filename, int n_topics, unsigned int n_words, vector<float> &phi)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_theta] Start reading phi\n");
    #endif

  FILE *fp = fopen(filename, "r");
  phi.resize(n_topics * n_words);
  for (int k = 0; k < n_topics; ++k)
  {
    for (unsigned int i = 0; i < n_words; ++i)
      fscanf(fp, "%f", &phi[k * n_words + i]);
  }
  fclose(fp);

    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_theta] Finish reading phi\n");
    #endif
}

void MyLDA::load_tassign(const char *filename, int n_docs, vector<pair<int, int> > &tassign, unsigned int n_words)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_tassign] Start reading tassign\n");
    #endif

  tassign.clear();
  tassign.resize(n_docs);
  FILE *fp = fopen(filename, "r");
  for (int k = 0; k < n_docs; ++k)
  {
    int wordid, topicid;
    for (unsigned int i = 0; i < n_words; ++i)
    {
      fscanf(fp, "%d:%d", &wordid, &topicid);//fscanf?
      tassign.push_back(make_pair(wordid, topicid));
    }
  }
  fclose(fp);

    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_tassign] Finish reading tassign\n");
    #endif
}

void MyLDA::load_wordmap(const char *filename, unsigned int& n_words, map<int, int> &wordmap, map<int, int> &reverse_wordmap)
{
    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_wordmap] Start reading wordmap\n");
    #endif

  wordmap.clear();
  FILE *fp = fopen(filename, "r");
  {
    unsigned int tmp_n_words;
    fscanf(fp, "%d", &tmp_n_words);
    n_words = tmp_n_words;
    fprintf(stderr, "n_words = %d, tmp_n_words = %d\n", n_words, tmp_n_words);
    // assert(tmp_n_words == n_words);
  }
  for (unsigned int i = 0; i < n_words; ++i)
  {
    int word, wordid;
    fscanf(fp, "%d%d", &word, &wordid);
    wordmap[word] = wordid;
    reverse_wordmap[wordid] = word;
  }
  fclose(fp);

    #ifndef LDA_SILENT
  fprintf(stderr, "[MyLDA::load_wordmap] Finish reading wordmap\n");
    #endif
}

