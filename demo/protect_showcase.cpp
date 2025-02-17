#include <mywr.hpp>

extern void showcase_protect();

void showcase_protect() {
  // In this example, we're simulating a situation where a plugin (DLL) is being
  // injected into the game process. The goal is to unprotect all of the game
  // image, allowing us to read, write, and execute code in the memory.

  // First, we need to get the handle to the executable module (the game
  // itself).
  const auto executable = mywr::module::get_executable_module_handle();

  // Next, we need to get the base address of the module (the game image).
  const auto allocation_base =
      mywr::module::get_module_allocation_base(executable);

  // Then, we need to get the size of the module (the game image).
  const auto image_size = mywr::module::get_module_image_size(allocation_base);

  // Now, we can check if we have both the base address and the size of the
  // module. If we do, we can use the mywr::protect::set_protect function to
  // unprotect the game image. In this example, we're setting the protection to
  // ReadWriteExecute, which allows us to read, write, and execute code in the
  // memory.
  if (allocation_base && image_size) {
    mywr::protect::set_protect(allocation_base,
                               mywr::protect::protection::ReadWriteExecute,
                               image_size);
  }
}
