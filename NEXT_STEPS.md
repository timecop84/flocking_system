# Next Steps: Force Combination Analysis

## Test Sequence for Improved Validation

1. **Launch Application**: ✅
2. **Press V in Legacy Mode**: Check if combined forces now match
3. **Toggle to Modern Mode**: Use checkbox  
4. **Press V in Modern Mode**: Compare modern vs legacy
5. **Analyze Results**: Look for specific differences

## Expected Improvements

After fixing the legacy validation to call `BehaviourSetup()` properly:
- **Combined Forces**: Should now show the correct legacy calculation
- **Modern vs Legacy**: May reveal more specific mismatches
- **Parameter Accuracy**: Modern uses actual legacy parameter values

## If Forces Still Don't Match

Possible remaining issues:
1. **Calculation Order**: Modern processes all boids at once, legacy one-by-one
2. **State Management**: Legacy may have persistent state between calculations
3. **Parameter Application**: Timing of when parameters are applied
4. **Vector Operations**: GLM vs ngl::Vector precision differences

## Next Migration Goals

1. **Complete Behavioral Parity**: Forces match exactly
2. **Performance Comparison**: Use P key to compare speeds
3. **Visual Verification**: Ensure flock behavior looks identical
4. **Modern Rendering**: Migrate OpenGL rendering to modern pipeline
5. **Final Cleanup**: Remove ngl_compat dependencies

## Success Criteria

✅ **Individual Forces Match**: Cohesion, Alignment, Separation
✅ **Combined Forces Match**: Total behavioral output identical  
✅ **Performance Acceptable**: Modern ≥ legacy performance
✅ **Visual Identical**: Same emergent flocking patterns
✅ **Code Quality**: Modern, maintainable, portable codebase
