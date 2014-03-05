## Version 1.2.0.5

 * More portability fixes.

 * Force stack alignment to 16 bytes everywhere. Fixes a segfault.

 * Fix bug where code relied on rewrite rules firing for correctness.

## Version1.2.0.4

 * Update docs to match code.

 * Work around bug in GHCi runtime linker, which never call static
   initializers.

## Version1.2.0.3

 * Make building of SSE 4.1 code conditional, as it doesn't work on all
  platforms.

 * Use a fixed salt, but allow random salting. Random salting by
  default broke people's code.

## Version1.2.0.2

 * Work around ghci linker bug on Windows.

## Version1.2.0.1

 * Fix performance bug in SSE implementation of SipHash.

 * Fix segfault due to incorrect stack alignment on Windows.

## Version1.2.0.0

 * Switch string hashing from FNV-1 to SipHash, in an effort to
   prevent collision attacks.

 * Switch fixed-size integer hashing to Wang hash.

 * The default salt now switched on every program run, in an effort to
   prevent collision attacks.

 * Move hash method out of Hashable type class.

 * Add support for generic instance deriving.

 * Add instance for Ordering.

## Version1.1.2.5

 * Bug fix for bytestring < 0.10.0.

## Version1.1.2.4

 * Switch string hashing from Bernstein to FNV-1

 * Faster instance for Integer.

 * Update dependency on base, ghc-prim

 * Now works with GHC 7.6.

## Version1.1.2.3

 * Add instance for TypeRep.

 * Update dependency on test-framework.

## Version1.1.2.2

 * Bug fix for GHC 7.4

## Version1.1.2.1

 * Update dependency on test-framework.

 * Improve documentation of combine.

## Version1.1.2.0

 * Add instances for Interger, Ratio, Float, Double, and StableName.

 * Fix hash collision issues for lists and tuples when using a
   user-specified salt.

## Version1.1.1.0

 * Improved instances for tuples and lists.

 * Add instances for StableName, Float, Double, Integer, and Ratio.

## Version1.1.1.0

 * Add hashWithSalt, which allows the user to create different hash
   values for the same input by providing different seeds. This is
   useful for application like Cuckoo hashing which need a family of
   hash functions.

 * Fix a bug in the Hashable instance for Int64/Word64 on 32-bit
   platforms.

 * Improved resilience to leading zero in the input being hashed.

## Version1.1.0.0

 * Add instance for: strict and lazy Texts, ThreadId

 * Add hashPtrWithSalt and hashByteArrayWithSalt.

 * Faster ByteArray# hashing.

 * Fix a signedness bug that affected ByteString.

 * Fix ByteString hashing to work correctly on both 32 and 64-bit
   platforms.

## Version1.0.1.1

 * Fix bug in Hashable instance for lazy ByteStrings where differences
   in the internal structure of the ByteString could cause different
   hash values for ByteStrings that are equal according to ==.

## Version1.0.1.0

 * Add two helpers for creating Hashable instances: hashPtr and
   hashByteArray.

## Version1.0.0

 * Separate Hashable class to its own package from hashmap 1.0.0.3.