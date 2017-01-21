#ifndef _MYLDA_H_
#define _MYLDA_H_

#include "GibbsLDA++-0.2/model.h"

#define LDA_SILENT

class LDAParameter{
public:
    LDAParameter(){};
    LDAParameter(int v_num_docs, int v_num_topics, int v_num_iters,
  float v_alpha, float v_beta);
    int num_docs;
    int num_topics;
    int num_iters;
    float alpha;
    float beta;
};

const int BASE_FEATURE = 0;
const int NUM_TOPICS = 15;
const int NUM_PW = 8;
const string DIR = "files/";
const float ALPHA = 5;
const float BETA = 0.1;
const int NUM_ITERS = 100;

class MyLDA : public model
{
public:
  int init(int model_status, float alpha, float beta, int n_topics, int n_iters, int savestep, int twords, string dir, string dfile, string model_name);

  bool run_model(string v_dir, string v_filename);
  void save_model();
  void load_model();
    MyLDA(){
        parameterReady = false;
    }

public:
    const string model_final_name = "model-final";
    string model_file;
    string trndata_file;
    string wordmap_file;
    string others_file;
    string theta_file;
    string phi_file;
    string tassign_file;
    string words_file;

    int _n_docs;
    unsigned int _n_words;
    int _n_topics;
    int _n_iters;
    int _total_wordids;
    std::vector<std::vector<int> > _words;

    std::vector<float> _theta;
    std::vector<float> _phi;
    std::vector<std::pair<int, int> > _tassign;
    std::map<int, int> _wordmap;  // word ==> wordid
    std::map<int, int> _reverse_wordmap;  // wordid ==> word

public:
    void getParameters(LDAParameter v_parameters);
    void save_words(const char *filename, int n_docs, const std::vector<vector<int> > &words);
    void load_others(const char *filename, float &alpha, float &beta, int &n_topics, int &n_docs, unsigned int &n_words);
    void load_words(const char *filename, int n_docs, std::vector<std::vector<int> > &words);
    void load_theta(const char *filename, int n_docs, int n_topics, std::vector<float> &theta);
    void load_phi(const char *filename, int n_topics, unsigned int n_words, std::vector<float> &phi);
    void load_tassign(const char *filename, int n_docs, vector<pair<int, int> > &tassign, unsigned int n_words);
    void load_wordmap(const char *filename, unsigned int &n_words, std::map<int, int> &wordmap, std::map<int, int> &reverse_wordmap);
private:
    bool parameterReady;
    float p_alpha;
    float p_beta;
};

#endif  // _WRAPPER_H_
