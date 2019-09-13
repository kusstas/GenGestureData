import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Data Gen")

    Flow {
        id: content

        anchors.fill: parent
        anchors.margins: 5

        spacing: 5

        ComboBox {
            id: typeDataComboBox
            model: ["Rotate Left", "Rotate Right", "To Down", "To Up"]
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Size:"
            }

            TextField {
                id: size
                width: 100
                text: "100x100"
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "FPS:"
            }

            TextField {
                id: fps
                width: 50
                text: "30"
            }
        }

        CheckBox {
            id: negativeCheckbox
            text: "Negative"
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Diameter:"
            }

            TextField {
                id: diam
                width: 100
                text: "10/10"
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Center:"
            }

            TextField {
                id: center
                width: 100
                text: "50x50"
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Max shift:"
            }

            TextField {
                id: maxShift
                width: 100
                text: "0x0"
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Diameter rotation/length move:"
            }

            TextField {
                id: valueData
                width: 100
                text: "25/25"
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Start angle:"
            }

            TextField {
                id: incline
                width: 100
                text: "-15/15"
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Chaotic:"
            }

            TextField {
                id: chaotic
                width: 75
                text: "0"

                validator: IntValidator {
                    bottom: 0
                }
            }
        }

        Row {
            spacing: 5
            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Count:"
            }

            TextField {
                id: count
                width: 50
                text: "1"

                validator: IntValidator {
                    bottom: 1
                }
            }
        }

        Button {
            text: "Generate"

            onClicked: {
                backend.setType(typeDataComboBox.currentText)
                backend.setSize(size.text)
                backend.setFps(fps.text)
                backend.setNegative(negativeCheckbox.checked)
                backend.setDiameter(diam.text)
                backend.setCenter(center.text)
                backend.setMaxShift(maxShift.text)
                backend.setValue(valueData.text)
                backend.setStartAngle(incline.text)
                backend.setChaotic(chaotic.text)
                backend.setCount(count.text)
                backend.generate()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: backend.busy

        MouseArea {
            anchors.fill: parent
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: visible
        }
    }
}
