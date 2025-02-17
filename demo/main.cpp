extern void showcase_address();
extern void showcase_protect();
extern void showcase_llmo();

auto main(int /*argc*/, char** /*argv*/) -> int {
  showcase_address();
  showcase_protect();
  showcase_llmo();

  return 0;
}
