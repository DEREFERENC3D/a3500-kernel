# A3500 kernel project

This is an attempt to reconstruct the history of, and clean up, the OEM-provided kernel source for the Lenovo A7-40 (A3500-FL Wi-Fi only model). This is the "inner" kernel repository, the original sources came in a zip file with the kernel in a `kernel` subdirectory (this repo), and an out-of-tree `mediatek` directory containing most of the hardware drivers and SoC support code, all wrapped in a custom make-based build system.

The main thing I was working for here was a small-as-possible diff of the OEM's changes. As such, I have prepared branches to cover the forking:

- `android-3.4-mtk-upstream` is the commit with the closest state to our sources, chosen as an older commit from the [generic Android kernel's official 3.4 branch (deprecated/android-3.4)](https://android.googlesource.com/kernel/common/+/refs/heads/deprecated/android-3.4),
- `android-3.4-tuxonice` is a merge of `android-3.4-mtk-upstream` and the TuxOnIce patches ([commit matching our exact kernel version](https://github.com/NigelCunningham/tuxonice-kernel/commit/746fe50dc8e670c6bc76249916e01866bc9e49fe)) which were also applied by the vendor,
- `android-3.4-mtk-base` shapes the repository state closer to the OEM sources, with cherry-picks, reverts and backports,
- `android-3.4-mtk-cleanup` contains the source state exactly as it came in the zip, the following commits clean it up,
- `android-3.4-mtk-clean` contains a checkout of the tip of `android-3.4-mtk-cleanup` as the merge commit, resulting in a cleaned up version (just over 100 files changed, down from a massive couple thousand), still compatible with the stock ROM and out-of-tree drivers, and build system.

The next steps will be merging in the out-of-tree component to drop the custom build system, then dropping a bunch of dead code (other SoC support, unused driverrs) and useless features (logging etc.) from it, to then clean up the diff some more. The end product should be something mergeable into a newer Android kernel to switch to a newer kernel version.
