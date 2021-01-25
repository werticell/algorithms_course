#include <vector>
#include <iostream>

static const int32_t FIELD_LENGTH = 8;
static const int32_t FIELD_SIZE = 64;
static const int32_t PLAYERS_NUMBER = 2;

struct State {
  int32_t term_pos_;
  int32_t runaway_pos_;
  char step_;
  State(int32_t t_pos, int32_t r_pos, char step) : term_pos_(t_pos),
                                                   runaway_pos_(r_pos),
                                                   step_(step) {}
};

enum Status {
  UNDEFINED,
  LOSING,
  WINNING
};


class GamePredictor {
 public:
  explicit GamePredictor(const std::vector<std::string>& field) : field_(field) {}

  void FindWinner();


 private:
  void dfs(int32_t t_pos, int32_t r_pos, char is_t_step);
  void BuildGraph();
  bool IsKillable(int32_t ti_pos, int32_t tj_pos,
                  int32_t ri_pos, int32_t rj_pos) const;

  void AddNeighbours(const State& prev_state, int32_t ti_pos, int32_t tj_pos,
                     int32_t ri_pos, int32_t rj_pos);

  void DoPrecalculus();

  std::pair<int32_t, int32_t> FindPlayersPos();

  const std::vector<std::string> field_;

  std::vector<State> graph_[FIELD_SIZE][FIELD_SIZE][PLAYERS_NUMBER];
  Status game_result_[FIELD_SIZE][FIELD_SIZE][PLAYERS_NUMBER] {UNDEFINED};
  bool used_[FIELD_SIZE][FIELD_SIZE][PLAYERS_NUMBER] {false};
  int32_t vertices_deg_[FIELD_SIZE][FIELD_SIZE][PLAYERS_NUMBER] {0};
};



void GamePredictor::dfs(int32_t t_pos, int32_t r_pos, char is_t_step) {
  used_[t_pos][r_pos][is_t_step] = true;

  for (const State& cur_state: graph_[t_pos][r_pos][is_t_step]) {
    if (!used_[cur_state.term_pos_][cur_state.runaway_pos_][cur_state.step_]) {

      if (game_result_[t_pos][r_pos][is_t_step] == LOSING) {
        game_result_[cur_state.term_pos_][cur_state.runaway_pos_][cur_state.step_] = WINNING;
      } else if (--vertices_deg_[cur_state.term_pos_][cur_state.runaway_pos_][cur_state.step_] == 0) {
        game_result_[cur_state.term_pos_][cur_state.runaway_pos_][cur_state.step_] = LOSING;
      } else {
        continue;
      }
      dfs(cur_state.term_pos_, cur_state.runaway_pos_, cur_state.step_);
    }
  }
}


std::pair<int32_t, int32_t>
GamePredictor::FindPlayersPos() {
  int32_t term_pos = 0, runaway_pos = 0;

  for (int32_t i = 0; i < FIELD_LENGTH; ++i) {
    for (int32_t j = 0; j < FIELD_LENGTH; ++j) {
      if (field_[i][j] == '3')
        term_pos = i * FIELD_LENGTH + j;
      else if (field_[i][j] == '2')
        runaway_pos = i * FIELD_LENGTH + j;
    }
  }
  return {term_pos, runaway_pos};
}

bool IsOnField(int32_t x, int32_t y) {
  return x >= 0 && y >= 0 && x < FIELD_LENGTH && y < FIELD_LENGTH;
}

bool
GamePredictor::IsKillable(int32_t ti_pos, int32_t tj_pos,
                          int32_t ri_pos, int32_t rj_pos) const {

  if (std::abs(rj_pos - tj_pos) == std::abs(ri_pos - ti_pos) || \
      rj_pos == tj_pos || ti_pos == ri_pos) {

    int32_t di = (ri_pos > ti_pos) ? 1 : ((ri_pos == ti_pos) ? 0 : -1);
    int32_t dj = (rj_pos > tj_pos) ? 1 : ((rj_pos == tj_pos) ? 0 : -1);

    for (int32_t i = 1; i < FIELD_LENGTH; ++i) {
      if (IsOnField(tj_pos + dj * i, ti_pos + di * i)) {
        if (field_[ti_pos + di * i][tj_pos + dj * i] == '1') {
          return false;
        } else if (field_[ti_pos + di * i][tj_pos + dj * i] == '2') {
          return true;
        }
      }
    }

    return true;
  } else {
    return false;
  }

}



void GamePredictor::BuildGraph() {
  for (int32_t t_pos = 0; t_pos < FIELD_LENGTH * FIELD_LENGTH; ++t_pos) {
    for (int32_t r_pos = 0; r_pos < FIELD_LENGTH * FIELD_LENGTH; ++r_pos) {
      for (int8_t is_t_step = 0; is_t_step < PLAYERS_NUMBER; ++is_t_step) {
        if (game_result_[t_pos][r_pos][is_t_step] != UNDEFINED && !used_[t_pos][r_pos][is_t_step])
          dfs(t_pos, r_pos, is_t_step);
      }
    }
  }
}


void
GamePredictor::AddNeighbours(const State& prev_state,
                             int32_t ti_pos, int32_t tj_pos,
                             int32_t ri_pos, int32_t rj_pos) {

  char is_t_step = prev_state.step_;
  const int32_t dj[] = {0, -1, 1, 0, 1, -1, -1, 1};
  const int32_t di[] = {-1, 0, 1, 1, -1, 1, -1, 0};
  for (int32_t k = 0; k < FIELD_LENGTH; ++k) {
    int32_t t_temp_j = tj_pos, t_temp_i = ti_pos;
    int32_t r_temp_j = rj_pos, r_temp_i = ri_pos;

    if (is_t_step == 0) {
      r_temp_j += dj[k];
      r_temp_i += di[k];
    } else {
      t_temp_j += dj[k];
      t_temp_i += di[k];
    }

    if (IsOnField(t_temp_j, t_temp_i) && IsOnField(r_temp_j, r_temp_i) && \
              field_[t_temp_i][t_temp_j] != '1' && field_[r_temp_i][r_temp_j] != '1') {

      graph_[t_temp_j + FIELD_LENGTH * t_temp_i]
      [r_temp_j + FIELD_LENGTH * r_temp_i]
      [is_t_step == 0].push_back(prev_state);
      ++vertices_deg_[prev_state.term_pos_][prev_state.runaway_pos_][is_t_step];
    }
  }
}


void GamePredictor::DoPrecalculus() {
  for (int32_t t_pos = 0; t_pos < FIELD_LENGTH * FIELD_LENGTH; ++t_pos) {
    for (int32_t r_pos = 0; r_pos < FIELD_LENGTH * FIELD_LENGTH; ++r_pos) {
      for (int8_t is_t_step = 0; is_t_step < PLAYERS_NUMBER; ++is_t_step) {
        int32_t tj_pos = t_pos % FIELD_LENGTH;
        int32_t ti_pos = t_pos / FIELD_LENGTH;
        int32_t rj_pos = r_pos % FIELD_LENGTH;
        int32_t ri_pos = r_pos / FIELD_LENGTH;

        if (field_[ti_pos][tj_pos] == '1' || field_[ri_pos][rj_pos]== '1') {
          continue;
        }

        Status& result = game_result_[t_pos][r_pos][is_t_step];
        if (is_t_step == 0) {
          result = (ri_pos == FIELD_LENGTH - 1 &&
                    !IsKillable(ti_pos, tj_pos, ri_pos, rj_pos)) ? WINNING : UNDEFINED;
          if (result != WINNING) {
            result = IsKillable(ti_pos, tj_pos, ri_pos, rj_pos) ? LOSING : UNDEFINED;
          }

        } else {
          result = (IsKillable(ti_pos, tj_pos, ri_pos, rj_pos)) ? WINNING : UNDEFINED;
        }

        if (result != UNDEFINED) {
          continue;
        }

        State current_state(t_pos, r_pos, is_t_step);
        AddNeighbours(current_state, ti_pos, tj_pos, ri_pos, rj_pos);
      }
    }
  }
}

void GamePredictor::FindWinner() {
  DoPrecalculus();
  BuildGraph();
  auto [term_pos, run_pos] = FindPlayersPos();
  std::cout << (game_result_[term_pos][run_pos][0] == WINNING ? 1 : -1);
}



int main() {
  std::vector<std::string> field(FIELD_LENGTH);
  for (int32_t i = 0; i < FIELD_LENGTH; ++i) {
    std::cin >> field[i];
  }

  GamePredictor game_predictor(field);
  game_predictor.FindWinner();

  return 0;
}
