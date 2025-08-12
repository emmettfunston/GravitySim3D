#version 330 core
out vec4 FragColor;
uniform vec3 cameraPos;

void main(){
    // Get screen coordinates
    vec2 screenPos = gl_FragCoord.xy;
    vec2 center = vec2(400.0, 300.0); // Center of screen
    vec2 offset = screenPos - center;
    
    // Create a grid pattern that will be deformed by gravity
    float gridSize = 20.0;
    vec2 grid = fract(offset / gridSize);
    
    // Calculate distance from center for gravity well effect
    float dist = length(offset) / 100.0;
    
    // Create the central gravity well - the net dips toward the center
    float centralWell = 1.0 / (1.0 + dist * 1.5);
    
    // Create local wells for each object (these would normally come from uniform variables)
    // Simulating the positions of the objects for the gravity well effect
    float localWell1 = 1.0 / (1.0 + length(offset - vec2(240.0, 0.0)) / 60.0); // Blue object
    float localWell2 = 1.0 / (1.0 + length(offset - vec2(360.0, 0.0)) / 60.0); // Purple object
    
    // Combine all gravity wells with different strengths
    float totalWell = centralWell + localWell1 * 0.6 + localWell2 * 0.4;
    
    // Apply gravity distortion to the grid - the net bends down where objects are
    vec2 gravityOffset = normalize(offset) * totalWell * 0.6;
    
    // Apply gravity distortion to the grid
    vec2 distortedGrid = grid + gravityOffset;
    float line = min(fract(distortedGrid.x), fract(distortedGrid.y));
    
    // Create smooth grid lines
    float gridLine = smoothstep(0.0, 0.015, line) * smoothstep(1.0, 0.985, line);
    
    // Create depth effect - the net appears to curve into the screen
    float depth = 1.0 - dist * 0.8;
    
    // Base grid color
    vec3 gridColor = mix(vec3(0.1, 0.1, 0.3), vec3(0.7, 0.7, 1.0), gridLine);
    
    // Add radial gradient for the gravity well
    vec3 radialColor = mix(vec3(0.05, 0.05, 0.15), vec3(0.2, 0.2, 0.5), depth);
    
    // Combine colors
    vec3 finalColor = mix(radialColor, gridColor, gridLine * 0.9);
    
    // Add central gravitational field effect (like a black hole)
    float centralField = 1.0 / (1.0 + dist * 2.5);
    finalColor += vec3(0.4, 0.4, 0.8) * centralField * 1.0;
    
    // Add local deformation effects where objects are - this makes the net appear malleable
    float localDeformation1 = localWell1 * 0.8;
    float localDeformation2 = localWell2 * 0.6;
    
    // Create visible dips in the net where objects are
    finalColor += vec3(0.3, 0.3, 0.7) * localDeformation1;
    finalColor += vec3(0.2, 0.2, 0.6) * localDeformation2;
    
    // Add ripple effects to show the net is responding to gravity
    float ripple = sin(dist * 30.0 + gl_FragCoord.x * 0.01) * 0.02;
    finalColor += vec3(0.1, 0.1, 0.3) * ripple;
    
    // Add some stars in the background
    vec2 starGrid = fract(offset / 60.0);
    float star = smoothstep(0.0, 0.005, starGrid.x) * smoothstep(1.0, 0.995, starGrid.x) * 
                 smoothstep(0.0, 0.005, starGrid.y) * smoothstep(1.0, 0.995, starGrid.y);
    finalColor += vec3(0.9, 0.9, 1.0) * star * 0.6;
    
    // Add atmospheric glow around the edges
    float edgeGlow = 1.0 - smoothstep(0.5, 1.0, dist);
    finalColor += vec3(0.1, 0.1, 0.4) * edgeGlow * 0.5;
    
    // Add depth shading to make the net appear 3D and malleable
    float netDepth = 1.0 - totalWell * 0.7;
    finalColor *= netDepth;
    
    // Add additional deformation visualization
    float deformation = localWell1 + localWell2;
    vec3 deformationColor = vec3(0.2, 0.2, 0.8) * deformation * 0.5;
    finalColor += deformationColor;
    
    FragColor = vec4(finalColor, 1.0);
} 