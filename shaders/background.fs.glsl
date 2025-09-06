#version 330 core
out vec4 FragColor;
uniform vec3 cameraPos;

void main(){
    // Get screen coordinates
    vec2 screenPos = gl_FragCoord.xy;
    vec2 center = vec2(400.0, 300.0); // Center of screen
    vec2 offset = screenPos - center;
    
    // Create a grid pattern that will be deformed by gravity
    float gridSize = 15.0; // Finer grid for better detail
    vec2 grid = fract(offset / gridSize);
    
    // Calculate distance from center for gravity well effect
    float dist = length(offset) / 80.0;
    
    // Create the central gravity well - the net dips toward the center
    float centralWell = 1.0 / (1.0 + dist * 1.2);
    
    // Create dynamic local wells that follow the objects
    // These simulate the objects moving around the net
    float time = gl_FragCoord.x * 0.001; // Simple time simulation
    
    // Object 1 (blue planet) - moving in circular orbit
    float angle1 = time * 0.5; // Orbital motion
    vec2 obj1Pos = vec2(cos(angle1) * 120.0, sin(angle1) * 120.0);
    float localWell1 = 1.0 / (1.0 + length(offset - obj1Pos) / 25.0);
    
    // Object 2 (purple planet) - moving in larger circular orbit
    float angle2 = time * 0.3; // Slower orbital motion
    vec2 obj2Pos = vec2(cos(angle2) * 180.0, sin(angle2) * 180.0);
    float localWell2 = 1.0 / (1.0 + length(offset - obj2Pos) / 25.0);
    
    // Object 3 (pink planet) - moving in largest circular orbit
    float angle3 = time * 0.2; // Slowest orbital motion
    vec2 obj3Pos = vec2(cos(angle3) * 240.0, sin(angle3) * 240.0);
    float localWell3 = 1.0 / (1.0 + length(offset - obj3Pos) / 25.0);
    
    // Object 4 (asteroid) - moving in elliptical orbit
    float angle4 = time * 0.4;
    vec2 obj4Pos = vec2(cos(angle4) * 80.0, sin(angle4) * 80.0 + 80.0);
    float localWell4 = 1.0 / (1.0 + length(offset - obj4Pos) / 20.0);
    
    // Combine all gravity wells with different strengths
    float totalWell = centralWell + localWell1 * 0.8 + localWell2 * 0.6 + localWell3 * 0.5 + localWell4 * 0.3;
    
    // Apply gravity distortion to the grid - the net bends down where objects are
    vec2 gravityOffset = normalize(offset) * totalWell * 0.6;
    
    // Apply gravity distortion to the grid
    vec2 distortedGrid = grid + gravityOffset;
    float line = min(fract(distortedGrid.x), fract(distortedGrid.y));
    
    // Create smooth grid lines
    float gridLine = smoothstep(0.0, 0.01, line) * smoothstep(1.0, 0.99, line);
    
    // Create depth effect - the net appears to curve into the screen
    float depth = 1.0 - dist * 0.9;
    
    // Base grid color
    vec3 gridColor = mix(vec3(0.1, 0.1, 0.3), vec3(0.8, 0.8, 1.0), gridLine);
    
    // Add radial gradient for the gravity well
    vec3 radialColor = mix(vec3(0.05, 0.05, 0.15), vec3(0.2, 0.2, 0.5), depth);
    
    // Combine colors
    vec3 finalColor = mix(radialColor, gridColor, gridLine * 0.9);
    
    // Add central gravitational field effect (like a black hole) - smaller and more localized
    float centralField = 1.0 / (1.0 + dist * 3.0); // More localized central well
    finalColor += vec3(0.4, 0.4, 0.8) * centralField * 0.8;
    
    // Create BLUE SHADOWS that follow each object - these show the net curvature
    // Each object gets its own blue shadow that moves with it
    
    // Blue shadow for object 1 (blue planet)
    float shadow1 = 1.0 / (1.0 + length(offset - obj1Pos) / 35.0);
    finalColor += vec3(0.0, 0.0, 0.8) * shadow1 * 0.7; // Strong blue shadow
    
    // Blue shadow for object 2 (purple planet)
    float shadow2 = 1.0 / (1.0 + length(offset - obj2Pos) / 35.0);
    finalColor += vec3(0.0, 0.0, 0.7) * shadow2 * 0.6; // Medium blue shadow
    
    // Blue shadow for object 3 (pink planet)
    float shadow3 = 1.0 / (1.0 + length(offset - obj3Pos) / 35.0);
    finalColor += vec3(0.0, 0.0, 0.6) * shadow3 * 0.5; // Light blue shadow
    
    // Blue shadow for object 4 (asteroid)
    float shadow4 = 1.0 / (1.0 + length(offset - obj4Pos) / 30.0);
    finalColor += vec3(0.0, 0.0, 0.5) * shadow4 * 0.4; // Subtle blue shadow
    
    // Add local deformation effects where objects are - this makes the net appear malleable
    float localDeformation1 = localWell1 * 1.0;
    float localDeformation2 = localWell2 * 0.8;
    float localDeformation3 = localWell3 * 0.6;
    float localDeformation4 = localWell4 * 0.4;
    
    // Create visible dips in the net where objects are with steep slopes
    finalColor += vec3(0.3, 0.3, 0.7) * localDeformation1;
    finalColor += vec3(0.2, 0.2, 0.6) * localDeformation2;
    finalColor += vec3(0.15, 0.15, 0.5) * localDeformation3;
    finalColor += vec3(0.1, 0.1, 0.4) * localDeformation4;
    
    // Add slope visualization - make lines steeper pointing toward objects
    float slope1 = 1.0 / (1.0 + length(offset - obj1Pos) / 15.0);
    float slope2 = 1.0 / (1.0 + length(offset - obj2Pos) / 15.0);
    float slope3 = 1.0 / (1.0 + length(offset - obj3Pos) / 15.0);
    float slope4 = 1.0 / (1.0 + length(offset - obj4Pos) / 12.0);
    
    // Create slope lines that point toward objects
    vec2 slopeDir1 = normalize(obj1Pos - offset);
    vec2 slopeDir2 = normalize(obj2Pos - offset);
    vec2 slopeDir3 = normalize(obj3Pos - offset);
    vec2 slopeDir4 = normalize(obj4Pos - offset);
    
    // Add slope lines to the grid
    float slopeLines1 = fract(dot(offset, slopeDir1) / 6.0);
    float slopeLines2 = fract(dot(offset, slopeDir2) / 6.0);
    float slopeLines3 = fract(dot(offset, slopeDir3) / 6.0);
    float slopeLines4 = fract(dot(offset, slopeDir4) / 5.0);
    
    float slopeLine1 = smoothstep(0.0, 0.005, slopeLines1) * smoothstep(1.0, 0.995, slopeLines1) * slope1;
    float slopeLine2 = smoothstep(0.0, 0.005, slopeLines2) * smoothstep(1.0, 0.995, slopeLines2) * slope2;
    float slopeLine3 = smoothstep(0.0, 0.005, slopeLines3) * smoothstep(1.0, 0.995, slopeLines3) * slope3;
    float slopeLine4 = smoothstep(0.0, 0.005, slopeLines4) * smoothstep(1.0, 0.995, slopeLines4) * slope4;
    
    // Add slope lines to show the net is dipping toward objects
    finalColor += vec3(0.5, 0.5, 0.9) * slopeLine1 * 0.6;
    finalColor += vec3(0.4, 0.4, 0.8) * slopeLine2 * 0.5;
    finalColor += vec3(0.3, 0.3, 0.7) * slopeLine3 * 0.4;
    finalColor += vec3(0.2, 0.2, 0.6) * slopeLine4 * 0.3;
    
    // Add ripple effects to show the net is responding to gravity
    float ripple = sin(dist * 35.0 + gl_FragCoord.x * 0.01) * 0.015;
    finalColor += vec3(0.1, 0.1, 0.3) * ripple;
    
    // Add some stars in the background
    vec2 starGrid = fract(offset / 50.0);
    float star = smoothstep(0.0, 0.004, starGrid.x) * smoothstep(1.0, 0.996, starGrid.x) * 
                 smoothstep(0.0, 0.004, starGrid.y) * smoothstep(1.0, 0.996, starGrid.y);
    finalColor += vec3(0.9, 0.9, 1.0) * star * 0.7;
    
    // Add atmospheric glow around the edges
    float edgeGlow = 1.0 - smoothstep(0.4, 1.0, dist);
    finalColor += vec3(0.1, 0.1, 0.4) * edgeGlow * 0.6;
    
    // Add depth shading to make the net appear 3D and malleable
    float netDepth = 1.0 - totalWell * 0.6;
    finalColor *= netDepth;
    
    // Add additional deformation visualization with steep slopes
    float deformation = localWell1 + localWell2 + localWell3 + localWell4;
    vec3 deformationColor = vec3(0.2, 0.2, 0.7) * deformation * 0.5;
    finalColor += deformationColor;
    
    // Add slope indicators - lines that show the direction of steepest descent
    float slopeIndicator = slope1 + slope2 + slope3 + slope4;
    finalColor += vec3(0.15, 0.15, 0.6) * slopeIndicator * 0.3;
    
    FragColor = vec4(finalColor, 1.0);
} 