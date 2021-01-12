# This can be used to create a Docker container with a prebuilt 3D Slicer and Slicer-LiverAnalysis plugin
# Run this to create a docker container called slicer-testbuild:
# docker build -t slicer-alive-build .

FROM slicer/slicer-base:latest
RUN \
	# Checkout latest
		cd /usr/src && \
		git clone https://github.com/ALive-research/Slicer.git && \
		cd Slicer && \
		#git pull && \
		# Make sure we have a version that actually builds
		#git checkout v4.11 && \
		
	# Re-configure
		#cd /usr/src/Slicer-build && \
		#cmake -DBUILD_TESTING:BOOL=OFF . && \
	# Build Slicer
		cd /usr/src/Slicer-build && \/usr/src/Slicer-build/BuildSlicer.sh && \
		
	# Build Alive plugin
	cd /usr/src && \
	git clone https://github.com/ALive-research/Slicer-LiverAnalysis.git && \
	mkdir Slicer-LiverAnalysis-Build && \
	cd Slicer-LiverAnalysis-Build && \
	cmake -DCMAKE_BUILD_TYPE:STRING=Release -DSlicer_DIR:PATH=/usr/src/Slicer-build/Slicer-build ../Slicer-LiverAnalysis && \
	make
		
WORKDIR /usr/src/Slicer-build
CMD bash