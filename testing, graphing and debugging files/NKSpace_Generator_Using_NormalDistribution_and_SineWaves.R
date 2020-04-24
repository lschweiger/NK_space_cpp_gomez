 library("ggplot2")

# For loop through space_number and k
for (i in seq(0,999,1)) {
# NK Space Number (ID)
space_number <- i # CHANGE --> Iterate 1 through 2^N
	for (k_opts in c(1,3,5,10)) {
	# Parameters
	N <- 15
	k <- k_opts #CHANGE --> Iterate through values of 1, 5, and 10

	# Solutons in the NK Space
	solutions_x <- seq(1,2^N) 

	# Analytic Approach
	# Addition of two sine functions with different periods and normal distribution 
	# then add perturbation

	## First Step | Define periods in terms of k and number of solutions
	wave.period.1 <- round(length(solutions_x)/2^k) #Period for first wave
	wave.period.2 <- round(length(solutions_x)/(2^(k+3))) #Period for second wave

	## Second Step | Create teh dataframes of solutions and sine functions
	wave.1 <- data.frame(Solutions=solutions_x,Scores=sin((1/wave.period.1)*(solutions_x)))
	wave.2 <- data.frame(Solutions=solutions_x,Scores=sin((1/wave.period.2)*(solutions_x)))

	## Third Step | Combine the two wave functions in random percentages
	### Define the random percentages
	random_proportion.1 <- runif(1, min=0, max=1)
	random_proportion.2 <- runif(1, min=0, max=1)
	### Add the two waves together by these percentages
	wave.3 <- data.frame(Solutions=solutions_x,
	                     Scores=random_proportion.1*wave.1$Scores+random_proportion.2*wave.2$Scores)
	### Remove negative values by adding the minimum value, bringing minimum to 0
	wave.3$Scores <- wave.3$Scores + abs(min(wave.3$Scores))
	### Now normalize by the max score and multiply the amplitude by 1/k
	wave.3$Scores <- (1/k)*wave.3$Scores/max(wave.3$Scores)

	wave.4 <- data.frame(Solutions=solutions_x,
	                     Scores=random_proportion.2*wave.2$Scores)
	### Remove negative values by adding the minimum value, bringing minimum to 0
	wave.4$Scores <- wave.4$Scores + abs(min(wave.4$Scores))
	### Now normalize by the max score and multiply the amplitude by 1/k
	wave.4$Scores <- (1/k)*wave.4$Scores/max(wave.4$Scores)


	### Visual Inspection
	ggplot(data=wave.3,aes(x=Solutions,y=Scores))+geom_line()
	ggplot(data=wave.4,aes(x=Solutions,y=Scores))+geom_line()
	## Fourth Step | Now create normal 
	### Pick random number to mean of normal, between 1 and 2^N
	random.mean <- round(runif(1,min=1,max=length(solutions_x)))
	### Create normal with mean and set standard deviation as SD/(2^K)
	standard_normal <- dnorm(solutions_x,mean=random.mean,sd=sd(solutions_x)/(2^(k)))
	### Normalize so maximum value is one
	standard_normal_normalized <- data.frame(Solutions=solutions_x,Scores=standard_normal/max(standard_normal))

	### Sanity Check
	ggplot(data=standard_normal_normalized,aes(x=Solutions,y=Scores))+geom_line()

	## Fifth Step | Now combine normal distribution with wave function by adding together
	standard_normal_normalized_wave_df <- data.frame(Solutions=solutions_x,
	                                                 Scores=standard_normal_normalized$Scores+wave.3$Scores)
	### Normalize combined normal and wave by maximum value
	standard_normal_normalized_wave_df$Scores <- standard_normal_normalized_wave_df$Scores/max(standard_normal_normalized_wave_df$Scores)

	### Sanity Check
	ggplot(data=standard_normal_normalized_wave_df,aes(x=Solutions,y=Scores))+geom_line()

	## Sixth Step | Pertub values for all 2^N values from a uniform between some values >0 and 0.3
	perturbation <- runif(max(solutions_x), # 2^N Values
	                      min=0.00005,
	                      max=0.30)
	### Add perturbation
	standard_normal_normalized_wave_df$Scores <- standard_normal_normalized_wave_df$Scores + perturbation
	### Remove negative values again by adding in minmum score
	standard_normal_normalized_wave_df$Scores <- standard_normal_normalized_wave_df$Scores + abs(min(standard_normal_normalized_wave_df$Scores))
	### Normalize again by maximum
	standard_normal_normalized_wave_df$Scores <- standard_normal_normalized_wave_df$Scores/max(standard_normal_normalized_wave_df$Scores)

	### Sanity Check | Visualize the Problem Space
	ggplot(data=standard_normal_normalized_wave_df,aes(x=Solutions,y=Scores))+geom_line()

	write.table(c(standard_normal_normalized_wave_df$Scores),
	          row.names=F,
	          col.names=F,
	          file = paste("4_NK_space_scores_",k,"_",space_number,"_R.txt",sep=""))
	}
}