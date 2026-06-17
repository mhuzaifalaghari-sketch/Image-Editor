#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QProcess>
#include <QWidget>
#include <QStackedWidget>
#include <iostream>
#include <QTimer>
#include <QDir>
#include <QSlider>
#include <QGroupBox>
#include <QDockWidget>


using namespace std;
QSlider* brightnessSlider;
QSlider* contrastSlider;
QLabel* brightnessValue;
QLabel* contrastValue;
QMainWindow *w = nullptr;
QString currentImagePath;
QLabel* currentImageView;
QString workingDir = "C:/Users/Huzaifa Laghari/Desktop/final__]/QtWidgetsApplication/x64/Debug/";
void sideMenu( QMainWindow* window)
{

	
	QDockWidget* sideMenu = new QDockWidget("Menu", window);
	sideMenu->setFeatures(QDockWidget::DockWidgetClosable |
		QDockWidget::DockWidgetMovable |
		QDockWidget::DockWidgetFloatable);

	// Create widget that will hold all menu items
	QWidget* menuWidget = new QWidget();
	QVBoxLayout* menuLayout = new QVBoxLayout(menuWidget);
	menuLayout->setAlignment(Qt::AlignTop);
	menuLayout->setSpacing(10);

	// ===== IMAGE ADJUSTMENTS =====
	QGroupBox* imageGroup = new QGroupBox("Image Adjustments");
	QVBoxLayout* imageLayout = new QVBoxLayout(imageGroup);

	// Brightness slider
	QLabel* brightnessLabel = new QLabel("Brightness:");
	brightnessSlider = new QSlider(Qt::Horizontal);
	brightnessSlider->setRange(-100 , 100);
	brightnessSlider->setValue(0);

	brightnessValue = new QLabel("0");
	brightnessValue->setAlignment(Qt::AlignCenter);

	QHBoxLayout* brightnessLayout = new QHBoxLayout();
	brightnessLayout->addWidget(brightnessLabel);
	brightnessLayout->addWidget(brightnessSlider);
	brightnessLayout->addWidget(brightnessValue);

	// Contrast slider
	QLabel* contrastLabel = new QLabel("Contrast:");
	contrastSlider = new QSlider(Qt::Horizontal);
	contrastSlider->setRange(-100, 100);
	contrastSlider->setValue(0);

	contrastValue = new QLabel("0");
	contrastValue->setAlignment(Qt::AlignCenter);

	QHBoxLayout* contrastLayout = new QHBoxLayout();
	contrastLayout->addWidget(contrastLabel);
	contrastLayout->addWidget(contrastSlider);
	contrastLayout->addWidget(contrastValue);

	imageLayout->addLayout(brightnessLayout);
	imageLayout->addLayout(contrastLayout);

	menuLayout->addWidget(imageGroup);
	sideMenu->setFixedWidth(350);
	sideMenu->setWidget(menuWidget);
	QPushButton* BandG = new QPushButton;
	BandG->setText("Add B&G Filter");
	BandG->setStyleSheet("QPushButton {"
		"    background-color: transparent;"
		"    color: #ffffff;"
		"    border: 2px solid #444444;"
		"    border-radius: 10px;"
		"    padding: 14px;"
		"    font-size: 16px;"
		"    font-weight: bold;"
		"    letter-spacing: 3px;"
		"    margin-top: 30px;"
		"}"
		"QPushButton:hover {"
		"    border: 2px solid #9900ff;"
		"    color: #9900ff;"
		"}"
		"QPushButton:pressed {"
		"    border: 2px solid #ff00d9;"
		"    color: #ff00d9;"
		"}");
	menuLayout->addWidget(BandG);
	window->addDockWidget(Qt::LeftDockWidgetArea,sideMenu);

	QTimer* throttleTimer = new QTimer();

	throttleTimer->setSingleShot(true);

	throttleTimer->setInterval(50);
	QObject::connect(brightnessSlider, &QSlider::valueChanged, [=](int value){

		brightnessValue->setText(QString::number(value));
		throttleTimer->start();
		
		});

	QObject::connect(throttleTimer, &QTimer::timeout, [=]() {
		// Call Python with current values
		QProcess* python = new QProcess();
		
		QStringList args;
		args << "C:/Users/Huzaifa Laghari/Desktop/final__]/QtWidgetsApplication/x64/Debug/image_processor.py";
		args << currentImagePath;
		args << QString::number(brightnessSlider->value());  // Brightness
		args << QString::number(contrastSlider->value());    // Contrast
		args << "0";  // No B&W
		// Add this line BEFORE python->start()
		python->setWorkingDirectory("C:/Users/Huzaifa Laghari/Desktop/final__]/QtWidgetsApplication/x64/Debug/");
		python->start("C:/ana/python.exe", args);

		QObject::connect(python, &QProcess::finished, [python](int exitCode) {
			if (exitCode == 0) {
				QString outputPath = python->readAllStandardOutput().trimmed();
				QString fullPath = workingDir + outputPath;
				currentImageView->setPixmap(QPixmap(outputPath));
				if (currentImageView && QFile::exists(fullPath)) {
					currentImageView->setPixmap(QPixmap(fullPath));
				}
			}

			python->deleteLater();
			});
		});



	// ADD THIS after the brightness slider connection:
	QObject::connect(contrastSlider, &QSlider::valueChanged, [=](int value) {
		contrastValue->setText(QString::number(value));
		throttleTimer->start();  
		});

	QObject::connect(BandG, &QPushButton::clicked, [=]() {
		if (currentImagePath.isEmpty()) return;

		QProcess* python = new QProcess();
		QStringList args;
		args << "C:/Users/Huzaifa Laghari/Desktop/final__]/QtWidgetsApplication/x64/Debug/image_processor.py";
		args << currentImagePath;
		args << "0";
		args << "0";
		args << "1";
		python->setWorkingDirectory("C:/Users/Huzaifa Laghari/Desktop/final__]/QtWidgetsApplication/x64/Debug/");

		python->start("C:/ana/python.exe", args);

		QObject::connect(python, &QProcess::finished, [python](int exitCode) {
			if (exitCode == 0) {

				QString outputPath = python->readAllStandardOutput().trimmed();
				QString fullPath = workingDir + outputPath;
				if (currentImageView && QFile::exists(fullPath)) {
					currentImageView->setPixmap(QPixmap(fullPath));
				}
			}
			python->deleteLater();
			});
		});


}



int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	app.setApplicationName("Image Editor");
	QStackedWidget* stackpages = new QStackedWidget(); 
	stackpages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	w = new QMainWindow;
	w->setCentralWidget(stackpages);
	w->showMaximized();
	w->setMinimumSize(400, 400);




	// page1

	QWidget *Login_page = new QWidget;
	Login_page->setStyleSheet(
		"QWidget {"
		"    background-color: #000000;"
		"}"
	);
	
	QVBoxLayout* Page1 = new QVBoxLayout(Login_page);
	Page1->setContentsMargins(40, 40, 40, 40);
	Page1->setSpacing(10);


	stackpages->addWidget(Login_page);
	stackpages->setCurrentIndex(0);
	QPushButton* Uploadimg_url = new QPushButton;
	Uploadimg_url->setText("Upload image Address");
	Uploadimg_url->setStyleSheet("QPushButton {"
		"    background-color: transparent;"
		"    color: #ffffff;"
		"    border: 2px solid #444444;"
		"    border-radius: 10px;"
		"    padding: 14px;"
		"    font-size: 16px;"
		"    font-weight: bold;"
		"    letter-spacing: 3px;"
		"    margin-top: 30px;"
		"}"
		"QPushButton:hover {"
		"    border: 2px solid #9900ff;"
		"    color: #9900ff;"
		"}"
		"QPushButton:pressed {"
		"    border: 2px solid #ff00d9;"
		"    color: #ff00d9;"
		"}");
	
	QLabel* Title = new QLabel("Enter Image Url");
	Title->setStyleSheet("QLabel {"
"    color: #bdc3c7;"
"    font-size: 36px;"
"    font-weight: 300;"
"    font-family: 'Segoe UI Light', 'Arial', sans-serif;"
"    background-color: transparent;"
"    letter-spacing: 3px;"
"    text-transform: uppercase;"
"}");
	Title->setAlignment(Qt::AlignHCenter);
		QLineEdit* adress = new  QLineEdit;
		adress->setPlaceholderText("Enter adress here");


		adress->setStyleSheet("    background-color: #1a1a1a;"
			"    border: 2px solid #333333;"
			"    border-radius: 8px;"
			"    padding: 12px;"
			"    font-size: 14px;"
			"    color: white;"
			"}"
			"QLineEdit:focus {"
			"    border: 2px solid #9900ff;"
			"}");
		adress->setAlignment(Qt::AlignHCenter);
		Page1->addWidget(Title);
		Page1->addWidget(adress);
		Page1->addSpacing(1);
		Page1->addWidget(Uploadimg_url);
		QObject::connect(Uploadimg_url, &QPushButton::clicked, [=]() {
			//page2
			QWidget *page2 = new QWidget;
			QVBoxLayout* con2 = new QVBoxLayout(page2);
			con2->setContentsMargins(0, 0, 0, 0);
			con2->setSpacing(0);
		
			QString adresss = adress->text();
			currentImagePath = adresss;
			QLabel* pic1 = new QLabel;

			pic1->setPixmap(QPixmap(adresss));
			currentImageView = pic1;
			pic1->setScaledContents(true);
			pic1->setMinimumSize(0, 0);
			pic1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			con2->addWidget(pic1);

			QPushButton* BandG = new QPushButton;
			BandG->setText("Add BandW filter");
			sideMenu(w);
			stackpages->addWidget(page2);
			stackpages->setCurrentIndex(1);
			
			});
		





		
	
		


	return app.exec();
}